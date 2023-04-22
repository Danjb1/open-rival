#include "pch.h"

#include "net/PacketFactory.h"

#include <iostream>
#include <string>

#include "net/packets/GameCommandPacket.h"
#include "EnumUtils.h"

namespace Rival {

std::shared_ptr<Packet> PacketFactory::deserialize(const std::vector<char>& buffer)
{
    // Read the PacketType from the buffer
    PacketType type = PacketType::Invalid;
    std::memcpy(&type, buffer.data(), sizeof(type));

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
