#include "pch.h"

#include "net/packets/AcceptPlayerPacket.h"

#include <cstddef>  // std::size_t
#include <string>

#include "utils/BufferUtils.h"
#include "GameCommand.h"

namespace Rival {

AcceptPlayerPacket::AcceptPlayerPacket(int requestId, std::string playerName, int playerId)
    : Packet(PacketType::AcceptPlayer)
    , requestId(requestId)
    , playerName(playerName)
    , playerId(playerId)
{
}

void AcceptPlayerPacket::serialize(std::vector<char>& buffer) const
{
    Packet::serialize(buffer);

    BufferUtils::addToBuffer(buffer, requestId);
    BufferUtils::addStringToBuffer(buffer, playerName);
    BufferUtils::addToBuffer(buffer, playerId);
}

std::shared_ptr<AcceptPlayerPacket> AcceptPlayerPacket::deserialize(const std::vector<char> buffer)
{
    std::size_t offset = relayedPacketHeaderSize;

    int requestId = 0;
    BufferUtils::readFromBuffer(buffer, offset, requestId);

    std::string playerName = BufferUtils::readStringFromBuffer(buffer, offset);

    int playerId = 0;
    BufferUtils::readFromBuffer(buffer, offset, playerId);

    return std::make_shared<AcceptPlayerPacket>(requestId, playerName, playerId);
}

}  // namespace Rival
