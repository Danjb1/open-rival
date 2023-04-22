#include "pch.h"

#include "net/packets/GameCommandPacket.h"

namespace Rival {

GameCommandPacket::GameCommandPacket(std::vector<std::shared_ptr<GameCommand>> commands, int tick)
    : commands(commands)
    , tick(tick)
{
}

void GameCommandPacket::serialize(std::vector<char>& buffer) const
{
    Packet::serialize(buffer);
}

std::shared_ptr<GameCommandPacket> GameCommandPacket::deserialize(const std::vector<char> buffer)
{
    // TODO: read data from buffer
    std::vector<std::shared_ptr<GameCommand>> commands;
    int tick = 0;
    return std::make_shared<GameCommandPacket>(commands, tick);
}

}  // namespace Rival
