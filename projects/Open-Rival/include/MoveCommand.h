#pragma once

#include <memory>

#include "GameCommand.h"

class MapNode;

namespace Rival {

class MoveCommand : public GameCommand
{
public:
    MoveCommand(int entityId, MapNode destination);

    // Begin GameCommand override
    void execute(GameCommandContext& context) override;
    // End GameCommand override

private:
    int entityId;
    MapNode destination;
};

}  // namespace Rival
