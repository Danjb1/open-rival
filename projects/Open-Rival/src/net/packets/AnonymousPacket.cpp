#include "pch.h"

#include "net/packets/AnonymousPacket.h"

#include <iostream>
#include <stdexcept>

#include "utils/BufferUtils.h"

namespace Rival {

AnonymousPacket::AnonymousPacket(const std::vector<char>& buffer, int playerId)
    // The packet type is not important, since we don't serialize it
    : Packet(PacketType::Invalid)
    , packetData(buffer)  // Make a copy of the incoming buffer
{
    setPlayerId(playerId);
}

void AnonymousPacket::serialize(std::vector<char>& buffer) const
{
    if (sizeof(int) + packetData.size() > buffer.capacity())
    {
        throw std::runtime_error("No room in packet for player ID");
    }

    // We inject the player ID before the packet data so clients can know who sent it
    BufferUtils::addToBuffer(buffer, playerId);

    // Fill buffer with packetData
    buffer.insert(std::end(buffer), std::cbegin(packetData), std::cend(packetData));
}

}  // namespace Rival
