#include "pch.h"

#include "net/packets/AnonymousPacket.h"

#include <stdexcept>

namespace Rival {

AnonymousPacket::AnonymousPacket(const std::vector<char>& buffer)
    : packetData(buffer)  // Make a copy of the incoming buffer
{
}

void AnonymousPacket::serialize(std::vector<char>& buffer) const
{
    // Fill buffer with packetData
    buffer.insert(std::end(buffer), std::begin(packetData), std::end(packetData));
}

}  // namespace Rival
