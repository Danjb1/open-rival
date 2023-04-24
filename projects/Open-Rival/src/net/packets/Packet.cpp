#include "pch.h"

#include "net/packets/Packet.h"

#include "utils/BufferUtils.h"

namespace Rival {

Packet::Packet(PacketType type, int playerId)
    : type(type)
    , playerId(playerId)
{
}

void Packet::serialize(std::vector<char>& buffer) const
{
    BufferUtils::addToBuffer(buffer, type);
    BufferUtils::addToBuffer(buffer, playerId);
}

}  // namespace Rival
