#include "net/packets/KickPlayerPacket.h"

#include <cstddef>  // std::size_t
#include <cstdlib>  // std::rand
#include <string>

#include "utils/BufferUtils.h"
#include "GameCommand.h"

namespace Rival {

KickPlayerPacket::KickPlayerPacket(int playerId)
    : Packet(PacketType::KickPlayer)
    , playerId(playerId)
{
}

void KickPlayerPacket::serialize(std::vector<char>& buffer) const
{
    Packet::serialize(buffer);

    BufferUtils::addToBuffer(buffer, playerId);
}

std::shared_ptr<KickPlayerPacket> KickPlayerPacket::deserialize(const std::vector<char> buffer)
{
    std::size_t offset = relayedPacketHeaderSize;

    int playerId = 0;
    BufferUtils::readFromBuffer(buffer, offset, playerId);

    return std::make_shared<KickPlayerPacket>(playerId);
}

}  // namespace Rival
