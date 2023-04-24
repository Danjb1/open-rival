#pragma once

#include <memory>
#include <vector>

#include "GameCommand.h"

class MapNode;

namespace Rival {

class MoveCommand : public GameCommand
{
public:
    MoveCommand(int entityId, MapNode destination);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<MoveCommand> deserialize(std::vector<char> buffer, size_t& offset);

    // Begin GameCommand override
    void execute(GameCommandContext& context) override;
    // End GameCommand override

private:
    int entityId;
    MapNode destination;
};

}  // namespace Rival
