#include "pch.h"

#include "commands/GameCommandFactory.h"

#include <iostream>
#include <string>

#include "utils/BufferUtils.h"
#include "EnumUtils.h"
#include "GameCommand.h"
#include "MoveCommand.h"

namespace Rival {

std::shared_ptr<GameCommand> GameCommandFactory::deserialize(const std::vector<char>& buffer, size_t& offset) const
{
    GameCommandType type = GameCommandType::Invalid;
    BufferUtils::readFromBuffer(buffer, offset, type);

    return deserializeFromType(buffer, type, offset);
}

std::shared_ptr<GameCommand>
GameCommandFactory::deserializeFromType(const std::vector<char>& buffer, GameCommandType type, size_t& offset) const
{
    switch (type)
    {
    case GameCommandType::Move:
        return MoveCommand::deserialize(buffer, offset);
    default:
        std::cerr << "Unsupported GameCommand type received: " << std::to_string(EnumUtils::toIntegral(type)) << "\n";
        return {};
    }
}

}  // namespace Rival
