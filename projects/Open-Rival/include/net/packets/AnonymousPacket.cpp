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

void AnonymousPacket::deserialize(const std::vector<char> buffer)
{
    // These should never be deserialized, they are always constructed directly
    throw std::runtime_error("Tried to deserialize anonymous packet");
}

}  // namespace Rival
