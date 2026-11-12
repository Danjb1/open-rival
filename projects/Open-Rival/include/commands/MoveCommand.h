#pragma once

#include <memory>
#include <vector>

#include "commands/GameCommand.h"
#include "game/MapUtils.h"

namespace Rival {

class Entity;

namespace Pathfinding {
class Context;
};

class MoveCommand : public GameCommand
{
public:
    MoveCommand(std::vector<int> entityIds, MapNode destination);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<MoveCommand> deserialize(std::vector<char> buffer, std::size_t& offset);

    // Begin GameCommand override
    void execute(GameCommandContext& context) override;
    // End GameCommand override

private:
    void moveEntities(const std::vector<Entity*>& entities, Pathfinding::Context& context);

private:
    std::vector<int> entityIds;
    MapNode destination;
};

}  // namespace Rival
