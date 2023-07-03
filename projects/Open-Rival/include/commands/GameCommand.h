#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "game/World.h"

namespace Rival {

enum class GameCommandType : std::uint8_t
{
    Invalid,
    Move
};

/**
 * Context provided to commands during execution.
 */
class GameCommandContext : public WorldStore
{
};

/**
 * Base class for a command that can be executed to affect the game.
 */
class GameCommand
{
public:
    GameCommand(GameCommandType type);
    virtual ~GameCommand() {}
    virtual void serialize(std::vector<char>& buffer) const;
    virtual void execute(GameCommandContext& context) = 0;

    GameCommandType getType() const
    {
        return type;
    }

private:
    GameCommandType type = GameCommandType::Invalid;
};

/**
 * Interface that allows invoking GameCommands.
 */
class GameCommandInvoker
{
public:
    virtual void dispatchCommand(std::shared_ptr<GameCommand> command) = 0;
};

}  // namespace Rival
