#include "pch.h"

#include "net/PacketFactory.h"

#include <iostream>
#include <string>

#include "net/packets/GameCommandPacket.h"
#include "utils/BufferUtils.h"
#include "EnumUtils.h"

namespace Rival {

std::shared_ptr<Packet> PacketFactory::deserialize(const std::vector<char>& buffer) const
{
    std::size_t offset = 0;

    int playerId = -1;
    BufferUtils::readFromBuffer(buffer, offset, playerId);

    PacketType type = PacketType::Invalid;
    BufferUtils::readFromBuffer(buffer, offset, type);

    std::shared_ptr<Packet> packet = deserializeFromType(buffer, type);
    if (packet)
    {
        packet->setPlayerId(playerId);
    }

    return packet;
}

std::shared_ptr<Packet> PacketFactory::deserializeFromType(const std::vector<char>& buffer, PacketType type) const
{
    switch (type)
    {
    case PacketType::GameCommand:
        return GameCommandPacket::deserialize(buffer, gameCommandFactory);
    default:
        std::cerr << "Unsupported packet type received: " << std::to_string(EnumUtils::toIntegral(type)) << "\n";
        return {};
    }
}

}  // namespace Rival
