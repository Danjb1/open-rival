#include "net/packets/RequestJoinPacket.h"

#include <cstddef>  // std::size_t
#include <string>

#include "utils/BufferUtils.h"
#include "commands/GameCommand.h"

namespace Rival {

RequestJoinPacket::RequestJoinPacket(int requestId, std::string playerName)
    : Packet(PacketType::RequestJoin)
    , requestId(requestId)
    , playerName(playerName)
{
}

void RequestJoinPacket::serialize(std::vector<char>& buffer) const
{
    Packet::serialize(buffer);

    BufferUtils::addToBuffer(buffer, requestId);
    BufferUtils::addStringToBuffer(buffer, playerName);
}

std::shared_ptr<RequestJoinPacket> RequestJoinPacket::deserialize(const std::vector<char> buffer)
{
    std::size_t offset = relayedPacketHeaderSize;

    int requestId = 0;
    BufferUtils::readFromBuffer(buffer, offset, requestId);

    std::string playerName = BufferUtils::readStringFromBuffer(buffer, offset);

    return std::make_shared<RequestJoinPacket>(requestId, playerName);
}

}  // namespace Rival
