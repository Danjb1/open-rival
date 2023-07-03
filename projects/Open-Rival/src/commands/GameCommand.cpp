#include "commands/GameCommand.h"

#include "utils/BufferUtils.h"

namespace Rival {

GameCommand::GameCommand(GameCommandType type)
    : type(type)
{
}

void GameCommand::serialize(std::vector<char>& buffer) const
{
    BufferUtils::addToBuffer(buffer, type);
}

}  // namespace Rival
