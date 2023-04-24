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
    BufferUtils::addToBuffer(buffer, type);
}

}  // namespace Rival
