#include "commands/GameCommandFactory.h"

#include <string>

#include "commands/GameCommand.h"
#include "commands/MoveCommand.h"
#include "utils/BufferUtils.h"
#include "utils/EnumUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

std::shared_ptr<GameCommand> GameCommandFactory::deserialize(const std::vector<char>& buffer, std::size_t& offset) const
{
    GameCommandType type = GameCommandType::Invalid;
    BufferUtils::readFromBuffer(buffer, offset, type);

    return deserializeFromType(buffer, type, offset);
}

std::shared_ptr<GameCommand> GameCommandFactory::deserializeFromType(
        const std::vector<char>& buffer, GameCommandType type, std::size_t& offset) const
{
    switch (type)
    {
    case GameCommandType::Move:
        return MoveCommand::deserialize(buffer, offset);
    default:
        LOG_WARN("Unsupported GameCommand type received: {}", EnumUtils::toIntegral(type));
        return {};
    }
}

}  // namespace Rival
