#include "net/packets/RelayedPacket.h"

#include <iostream>
#include <stdexcept>

#include "utils/BufferUtils.h"

namespace Rival {

RelayedPacket::RelayedPacket(const std::vector<char>& buffer, int clientId)
    // The packet type is not important, since we don't serialize it
    : Packet(PacketType::Invalid)
    , packetData(buffer)  // Make a copy of the incoming buffer
{
    setClientId(clientId);
}

void RelayedPacket::serialize(std::vector<char>& buffer) const
{
    // Make sure the buffer is big enough for us to inject the client ID
    std::size_t packetSize = Packet::sizeBytes + sizeof(clientId) + packetData.size();
    if (packetSize > buffer.capacity())
    {
        throw std::runtime_error("No room in packet for client ID");
    }

    // Placeholder for packet size
    BufferUtils::addToBuffer(buffer, 0);

    // We inject the client ID before the packet data so clients can know who sent it
    BufferUtils::addToBuffer(buffer, clientId);

    // Fill buffer with packetData
    buffer.insert(std::end(buffer), std::cbegin(packetData), std::cend(packetData));
}

}  // namespace Rival
