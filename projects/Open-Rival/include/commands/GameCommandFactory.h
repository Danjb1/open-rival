#pragma once

#include <memory>
#include <vector>

#include "GameCommand.h"

namespace Rival {

/**
 * Deserializes GameCommands from raw data.
 */
class GameCommandFactory
{
public:
    std::shared_ptr<GameCommand> deserialize(const std::vector<char>& buffer, std::size_t& offset) const;

private:
    std::shared_ptr<GameCommand>
    deserializeFromType(const std::vector<char>& buffer, GameCommandType type, std::size_t& offset) const;
};

}  // namespace Rival
