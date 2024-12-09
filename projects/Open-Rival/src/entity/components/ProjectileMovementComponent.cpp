#include "entity/components/ProjectileMovementComponent.h"

namespace Rival {

const std::string ProjectileMovementComponent::key = "projectile_movement";

ProjectileMovementComponent::ProjectileMovementComponent()
    : EntityComponent(key)
{
    // TODO: Start and end position
    // TODO: Projectile speed
}

}  // namespace Rival
