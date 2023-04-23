#include "pch.h"

#include "net/packets/AnonymousPacket.h"

#include <iostream>
#include <stdexcept>

namespace Rival {

AnonymousPacket::AnonymousPacket(const std::vector<char>& buffer)
    // The packet type is not important, since we don't serialize it
    : Packet(PacketType::Invalid)
    , packetData(buffer)  // Make a copy of the incoming buffer
{
}

void AnonymousPacket::serialize(std::vector<char>& buffer) const
{
    // Fill buffer with packetData (buffer should be empty at this point)
    buffer.insert(std::begin(buffer), std::cbegin(packetData), std::cend(packetData));
}

}  // namespace Rival
