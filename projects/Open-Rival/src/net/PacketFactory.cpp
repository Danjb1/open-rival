#include "pch.h"

#include "net/PacketFactory.h"

#include <iostream>
#include <string>

#include "net/packets/AcceptPlayerPacket.h"
#include "net/packets/GameCommandPacket.h"
#include "net/packets/KickPlayerPacket.h"
#include "net/packets/RejectPlayerPacket.h"
#include "net/packets/RequestJoinPacket.h"
#include "net/packets/StartGamePacket.h"
#include "utils/BufferUtils.h"
#include "EnumUtils.h"

namespace Rival {

std::shared_ptr<Packet> PacketFactory::deserialize(const std::vector<char>& buffer) const
{
    std::size_t offset = 0;

    int clientId = -1;
    BufferUtils::readFromBuffer(buffer, offset, clientId);

    PacketType type = PacketType::Invalid;
    BufferUtils::readFromBuffer(buffer, offset, type);

    std::shared_ptr<Packet> packet = deserializeFromType(buffer, type);
    if (packet)
    {
        packet->setClientId(clientId);
    }

    return packet;
}

std::shared_ptr<Packet> PacketFactory::deserializeFromType(const std::vector<char>& buffer, PacketType type) const
{
    switch (type)
    {
    case PacketType::RequestJoin:
        return RequestJoinPacket::deserialize(buffer);
    case PacketType::AcceptPlayer:
        return AcceptPlayerPacket::deserialize(buffer);
    case PacketType::RejectPlayer:
        return RejectPlayerPacket::deserialize(buffer);
    case PacketType::KickPlayer:
        return KickPlayerPacket::deserialize(buffer);
    case PacketType::StartGame:
        return StartGamePacket::deserialize(buffer);
    case PacketType::GameCommand:
        return GameCommandPacket::deserialize(buffer, gameCommandFactory);
    default:
        std::cerr << "Unsupported packet type received: " << std::to_string(EnumUtils::toIntegral(type)) << "\n";
        return {};
    }
}

}  // namespace Rival
