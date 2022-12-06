#pragma once

#include <memory>

#include "World.h"

namespace Rival {

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
    virtual void execute(GameCommandContext& context) = 0;
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
