#include "pch.h"

#include "net/packets/RequestJoinPacket.h"

#include <cstddef>  // std::size_t
#include <string>

#include "utils/BufferUtils.h"
#include "GameCommand.h"

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

    BufferUtils::addToBuffer(buffer, playerName.size());
    for (size_t i = 0; i < playerName.size(); ++i)
    {
        BufferUtils::addToBuffer(buffer, playerName.c_str()[i]);
    }
}

std::shared_ptr<RequestJoinPacket> RequestJoinPacket::deserialize(const std::vector<char> buffer)
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

    return std::make_shared<RequestJoinPacket>(requestId, playerName);
}

}  // namespace Rival
