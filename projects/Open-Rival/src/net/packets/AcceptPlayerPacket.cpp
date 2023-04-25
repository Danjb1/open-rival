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

    BufferUtils::addToBuffer(buffer, playerName.size());
    for (size_t i = 0; i < playerName.size(); ++i)
    {
        BufferUtils::addToBuffer(buffer, playerName.c_str()[i]);
    }

    BufferUtils::addToBuffer(buffer, playerId);
}

std::shared_ptr<AcceptPlayerPacket> AcceptPlayerPacket::deserialize(const std::vector<char> buffer)
{
    std::size_t offset = relayedPacketHeaderSize;

    int requestId = 0;
    BufferUtils::readFromBuffer(buffer, offset, requestId);

    size_t playerNameSize = 0;
    BufferUtils::readFromBuffer(buffer, offset, playerNameSize);

    std::string playerName;
    playerName.reserve(playerNameSize);
    for (size_t i = 0; i < playerNameSize; ++i)
    {
        char c;
        BufferUtils::readFromBuffer(buffer, offset, c);
        playerName += c;
    }

    int playerId = 0;
    BufferUtils::readFromBuffer(buffer, offset, playerId);

    return std::make_shared<AcceptPlayerPacket>(requestId, playerName, playerId);
}

}  // namespace Rival
