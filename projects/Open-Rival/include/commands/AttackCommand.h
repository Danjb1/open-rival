#pragma once

#include <memory>
#include <vector>

#include "commands/GameCommand.h"
#include "game/MapUtils.h"

namespace Rival {

/** Command that causes one or more units to attack.
 * The target could be an entity, or a tile. */
class AttackCommand : public GameCommand
{
public:
    AttackCommand(std::vector<int> entityIds, int targetEntityId);
    AttackCommand(std::vector<int> entityIds, MapNode targetTile);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<AttackCommand> deserialize(std::vector<char> buffer, std::size_t& offset);

    // Begin GameCommand override
    void execute(GameCommandContext& context) override;
    // End GameCommand override

private:
    std::vector<int> entityIds;
    int targetEntityId;
    MapNode targetTile;
};

}  // namespace Rival
