#include "pch.h"

#include "net/packets/GameCommandPacket.h"

#include <cstddef>  // std::size_t
#include <cstdint>
#include <iostream>
#include <string>

#include "utils/BufferUtils.h"
#include "GameCommand.h"

namespace Rival {

GameCommandPacket::GameCommandPacket(int playerId, std::vector<std::shared_ptr<GameCommand>> commands, int tick)
    : Packet(PacketType::GameCommand, playerId)
    , commands(commands)
    , tick(tick)
{
}

void GameCommandPacket::serialize(std::vector<char>& buffer) const
{
    Packet::serialize(buffer);

    BufferUtils::addToBuffer(buffer, static_cast<std::uint8_t>(commands.size()));
    BufferUtils::addToBuffer(buffer, tick);

    for (auto& command : commands)
    {
        command->serialize(buffer);
    }
}

std::shared_ptr<GameCommandPacket> GameCommandPacket::deserialize(const std::vector<char> buffer)
{
    std::size_t offset = packetDataOffset;

    int playerId = -1;
    BufferUtils::readFromBuffer(buffer, offset, playerId);

    std::uint8_t numCommands = 0;
    BufferUtils::readFromBuffer(buffer, offset, numCommands);

    int tick = 0;
    BufferUtils::readFromBuffer(buffer, offset, tick);

    std::vector<std::shared_ptr<GameCommand>> commands;
    for (std::uint8_t i = 0; i < numCommands; ++i)
    {
        GameCommandType type = GameCommandType::Invalid;
        BufferUtils::readFromBuffer(buffer, offset, type);

        // TODO: deserialize command (use a GameCommandFactory?)
    }

    return std::make_shared<GameCommandPacket>(playerId, commands, tick);
}

}  // namespace Rival
