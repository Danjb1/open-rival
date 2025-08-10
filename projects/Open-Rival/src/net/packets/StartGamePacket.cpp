#include "net/packets/StartGamePacket.h"

#include <cstddef>  // std::size_t
#include <string>

#include "utils/BufferUtils.h"

namespace Rival {

StartGamePacket::StartGamePacket()
    : Packet(PacketType::StartGame)
{
}

std::shared_ptr<StartGamePacket> StartGamePacket::deserialize(const std::vector<char> /*buffer*/)
{
    return std::make_shared<StartGamePacket>();
}

}  // namespace Rival
