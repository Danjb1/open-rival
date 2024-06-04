#pragma once

#include <string>

#include "entity/components/EntityComponent.h"
#include "game/Tile.h"

namespace Rival {

class World;

/**
 * Component that affects the passability of an entity's tile.
 */
class PassabilityComponent : public EntityComponent
{

public:
    static const std::string key;

    PassabilityComponent(TilePassability passability);

    // Begin EntityComponent override
    void onEntityAddedToWorld(World* world) override;
    void onEntityRemovedFromWorld(World* world) override;
    // End EntityComponent override

    TilePassability getPassability() const;

private:
    TilePassability passability;
};

}  // namespace Rival
