#include "net/Connection.h"

#include <stdexcept>
#include <string>

#include "net/packets/RelayedPacket.h"
#include "utils/BufferUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

Connection::Connection(
        Socket socket, std::shared_ptr<PacketFactory> packetFactory, int remoteClientId, ConnectionListener* listener)
    : socket(std::move(socket))
    , packetFactory(packetFactory)
    , remoteClientId(remoteClientId)
    , listener(listener)
    , sendThread(&Connection::sendThreadLoop, this)
    , receiveThread(&Connection::receiveThreadLoop, this)
    , state(ConnectionState::Open)
{
    sendBuffer.reserve(maxBufferSize);
    recvBuffer.reserve(maxBufferSize);
}

Connection::~Connection()
{
    close();
}

bool Connection::operator==(const Connection& other) const
{
    return socket == other.socket;
}

bool Connection::operator!=(const Connection& other) const
{
    return !(*this == other);
}

void Connection::close() noexcept
{
    if (state != ConnectionState::Open)
    {
        // Close has already been requested
        return;
    }

    LOG_DEBUG("Closing connection...");
    state = ConnectionState::Closing;

    // Try to close the underlying socket, although it may well be closed already
    socket.close();

    // Forcibly wake the send thread so it can re-evaluate its end condition
    {
        // The mutex is necessary here to avoid the "lost wake-up" problem, where we send the notify in the instant
        // before the call to `wait` and it gets missed.
        std::lock_guard<std::mutex> lock(packetsToSendMutex);
        sendReadyCondition.notify_all();
    }

    // Stop send thread
    if (sendThread.joinable())
    {
        LOG_TRACE("Waiting for connection send thread to finish");
        sendThread.join();
    }

    // Stop receive thread
    if (receiveThread.joinable())
    {
        LOG_TRACE("Waiting for connection receive thread to finish");
        receiveThread.join();
    }

    state = ConnectionState::Closed;
    LOG_DEBUG("Connection successfully closed!");
}

bool Connection::isOpen() const
{
    return socket.isOpen();
}

void Connection::send(const std::shared_ptr<const Packet> packet)
{
    {
        std::scoped_lock lock(packetsToSendMutex);
        packetsToSend.push_back(packet);
    }

    // Wake up the send thread
    sendReadyCondition.notify_one();
}

void Connection::sendThreadLoop()
{
    while (isOpen())
    {
        std::shared_ptr<const Packet> packet;

        {
            std::unique_lock<std::mutex> lock(packetsToSendMutex);

            // Wait until we have a packet to send OR the connection closes
            sendReadyCondition.wait(lock, [this] { return !packetsToSend.empty() || !isOpen(); });

            if (!isOpen())
            {
                break;
            }

            packet = packetsToSend.front();
            packetsToSend.pop_front();
        }

        if (packet)
        {
            sendNow(packet);
        }
    }
}

void Connection::receiveThreadLoop()
{
    while (isOpen())
    {
        // First read the packet size
        if (!readFromSocket(Packet::sizeBytes))
        {
            break;
        }

        // Extract the packet size from the buffer
        std::size_t offset = 0;
        int nextPacketSize = 0;
        BufferUtils::readFromBuffer(recvBuffer, offset, nextPacketSize);
        recvBuffer.clear();

        // Sanity-check the packet size
        if (nextPacketSize > maxBufferSize)
        {
            throw std::runtime_error("Unexpected packet size: " + std::to_string(nextPacketSize));
        }

        // Now read the packet itself
        if (!readFromSocket(nextPacketSize))
        {
            break;
        }

        // If this Connection has no packet factory, then it belongs to the relay server. The relay server doesn't care
        // about the contents of incoming packets, it just wraps them in RelayedPackets.
        std::shared_ptr<const Packet> packet = packetFactory
                ? packetFactory->deserialize(recvBuffer)
                : std::make_shared<RelayedPacket>(recvBuffer, remoteClientId);

        if (packet)
        {
            // Pass packets directly to the listener if present, otherwise queue them until requested
            if (listener)
            {
                listener->onPacketReceived(*this, packet);
            }
            else
            {
                std::scoped_lock lock(receivedPacketsMutex);
                receivedPackets.push_back(packet);
            }
        }
    }
}

bool Connection::readFromSocket(std::size_t numBytes)
{
    recvBuffer.resize(numBytes);
    socket.receive(recvBuffer);

    // The socket may get closed during a call to `receive`
    bool success = isOpen();
    return success;
}

void Connection::sendNow(std::shared_ptr<const Packet> packet)
{
    packet->serialize(sendBuffer);
    packet->finalize(sendBuffer);

    if (sendBuffer.empty())
    {
        throw std::runtime_error("Tried to send empty buffer");
    }

    socket.send(sendBuffer);
    sendBuffer.clear();
}

std::vector<std::shared_ptr<const Packet>> Connection::getReceivedPackets()
{
    std::vector<std::shared_ptr<const Packet>> packetsToReturn;

    {
        std::scoped_lock lock(receivedPacketsMutex);
        packetsToReturn = receivedPackets;
        receivedPackets.clear();
    }

    return packetsToReturn;
}

void Connection::returnPackets(std::vector<std::shared_ptr<const Packet>>& unprocessedPackets)
{
    std::scoped_lock lock(receivedPacketsMutex);
    receivedPackets.insert(receivedPackets.cbegin(), unprocessedPackets.cbegin(), unprocessedPackets.cend());
}

}  // namespace Rival
