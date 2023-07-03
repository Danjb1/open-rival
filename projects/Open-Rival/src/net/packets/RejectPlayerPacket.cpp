#include "net/packets/RejectPlayerPacket.h"

#include <cstddef>  // std::size_t
#include <string>

#include "utils/BufferUtils.h"
#include "commands/GameCommand.h"

namespace Rival {

RejectPlayerPacket::RejectPlayerPacket(int requestId, std::string playerName)
    : Packet(PacketType::RejectPlayer)
    , requestId(requestId)
    , playerName(playerName)
{
}

void RejectPlayerPacket::serialize(std::vector<char>& buffer) const
{
    Packet::serialize(buffer);

    BufferUtils::addToBuffer(buffer, requestId);

    BufferUtils::addToBuffer(buffer, playerName.size());
    for (std::size_t i = 0; i < playerName.size(); ++i)
    {
        BufferUtils::addToBuffer(buffer, playerName.c_str()[i]);
    }
}

std::shared_ptr<RejectPlayerPacket> RejectPlayerPacket::deserialize(const std::vector<char> buffer)
{
    std::size_t offset = relayedPacketHeaderSize;

    int requestId = 0;
    BufferUtils::readFromBuffer(buffer, offset, requestId);

    std::size_t playerNameSize = 0;
    BufferUtils::readFromBuffer(buffer, offset, playerNameSize);

    std::string playerName;
    playerName.reserve(playerNameSize);
    for (std::size_t i = 0; i < playerNameSize; ++i)
    {
        char c;
        BufferUtils::readFromBuffer(buffer, offset, c);
        playerName += c;
    }

    return std::make_shared<RejectPlayerPacket>(requestId, playerName);
}

}  // namespace Rival
