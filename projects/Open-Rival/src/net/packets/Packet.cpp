#include "pch.h"

#include "net/packets/Packet.h"

#include "utils/BufferUtils.h"

namespace Rival {

Packet::Packet(PacketType type)
    : type(type)
{
}

void Packet::serialize(std::vector<char>& buffer) const
{
    BufferUtils::addToBuffer(buffer, 0);  // Placeholder for packet size
    BufferUtils::addToBuffer(buffer, type);
}

void Packet::finalize(std::vector<char>& buffer) const
{
    // Overwrite the placeholder packet size in the buffer
    int packetSize = static_cast<int>(buffer.size() - sizeof(packetSize));
    std::memcpy(buffer.data(), &packetSize, sizeof(packetSize));
}

}  // namespace Rival
