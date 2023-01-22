#include "pch.h"

#include "net/Connection.h"

namespace Rival {

Connection::Connection(Socket socket, std::shared_ptr<PacketFactory> packetFactory)
    : socket(std::move(socket))
    , packetFactory(packetFactory)
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

void Connection::close() noexcept
{
    socket.close();
}

void Connection::receiveThreadLoop()
{
    while (!socket.isClosed())
    {
        socket.receive(recvBuffer);

        std::shared_ptr<const Packet> packet = packetFactory->deserialize(recvBuffer);

        {
            std::scoped_lock lock(receivedPacketsMutex);
            receivedPackets.push_back(packet);
        }

        recvBuffer.clear();
    }
}

void Connection::send(const Packet& packet)
{
    packet.serialize(sendBuffer);
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
