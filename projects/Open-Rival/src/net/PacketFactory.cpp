#include "pch.h"

#include "net/PacketFactory.h"

#include <iostream>
#include <string>

#include "net/packets/GameCommandPacket.h"
#include "utils/BufferUtils.h"
#include "EnumUtils.h"

namespace Rival {

std::shared_ptr<Packet> PacketFactory::deserialize(const std::vector<char>& buffer)
{
    std::size_t offset = 0;

    PacketType type = PacketType::Invalid;
    BufferUtils::readFromBuffer(buffer, offset, type);

    return deserializeFromType(buffer, type);
}

std::shared_ptr<Packet> PacketFactory::deserializeFromType(const std::vector<char>& buffer, PacketType type)
{
    switch (type)
    {
    case PacketType::GameCommand:
        return GameCommandPacket::deserialize(buffer);
    default:
        std::cerr << "Unsupported packet type received: " << std::to_string(EnumUtils::toIntegral(type)) << "\n";
        return {};
    }
}

}  // namespace Rival
