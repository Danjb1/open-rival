#pragma once

#include <glm/vec2.hpp>

#include <string>

#include "entity/components/EntityComponent.h"
#include "game/MapUtils.h"

namespace Rival {

/**
 * Component that moves a projectile to its destination.
 */
class ProjectileMovementComponent : public EntityComponent
{

public:
    ProjectileMovementComponent(MapNode target);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void update(int delta) override;
    // End EntityComponent override

    glm::vec2 getRenderOffset(int delta) const;

private:
    void onProjectileArrived();

public:
    static const std::string key;

private:
    MapNode target;
    int lifetime = 0;
    int timeElapsed = 0;
};

}  // namespace Rival
