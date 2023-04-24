#include "pch.h"

#include "net/Connection.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "net/packets/AnonymousPacket.h"
#include "utils/BufferUtils.h"

namespace Rival {

Connection::Connection(Socket socket, std::shared_ptr<PacketFactory> packetFactory, ConnectionListener* listener)
    : socket(std::move(socket))
    , packetFactory(packetFactory)
    , listener(listener)
    , receiveThread(&Connection::receiveThreadLoop, this)
{
    sendBuffer.reserve(bufferSize);
    recvBuffer.reserve(bufferSize);
}

Connection::~Connection()
{
    close();
    receiveThread.join();
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
    socket.close();
}

bool Connection::isClosed() const
{
    return socket.isClosed();
}

void Connection::receiveThreadLoop()
{
    while (!socket.isClosed())
    {
        recvBuffer.resize(bufferSize);
        socket.receive(recvBuffer);

        // If this Connection has no packet factory, just wrap the buffers in anonymous packets. These are used by the
        // relay server, which doesn't care about their contents.
        std::shared_ptr<const Packet> packet =
                packetFactory ? packetFactory->deserialize(recvBuffer) : makeAnonymousPacket(recvBuffer);

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

        recvBuffer.clear();
    }
}

std::shared_ptr<const Packet> Connection::makeAnonymousPacket(const std::vector<char>& buffer)
{
    // Peek at the packet header to validate the player ID
    int playerId = -1;
    size_t offset = Packet::playerIdOffset;
    BufferUtils::readFromBuffer(buffer, offset, playerId);

    if (playerId != remotePlayerId)
    {
        std::cerr << "Server received packet with incorrect player ID\n";
        return {};
    }

    return std::make_shared<AnonymousPacket>(recvBuffer);
}

void Connection::send(const Packet& packet)
{
    packet.serialize(sendBuffer);
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

}  // namespace Rival
