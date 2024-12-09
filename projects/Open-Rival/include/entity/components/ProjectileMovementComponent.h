#pragma once

#include <string>

#include "entity/components/EntityComponent.h"

namespace Rival {

/**
 * Component that moves a projectile to its destination.
 */
class ProjectileMovementComponent : public EntityComponent
{

public:
    ProjectileMovementComponent();

public:
    static const std::string key;
};

}  // namespace Rival
