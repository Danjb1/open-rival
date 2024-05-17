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

    void onEntityAddedToWorld(World* world) override;

    TilePassability getPassability() const;

private:
    TilePassability passability;
};

}  // namespace Rival
