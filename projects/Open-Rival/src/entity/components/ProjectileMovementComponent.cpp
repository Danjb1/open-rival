#include "entity/components/ProjectileMovementComponent.h"

#include "entity/Projectile.h"
#include "gfx/RenderUtils.h"
#include "utils/LogUtils.h"
#include "utils/MathUtils.h"

namespace Rival {

const std::string ProjectileMovementComponent::key = "projectile_movement";

ProjectileMovementComponent::ProjectileMovementComponent(MapNode target)
    : EntityComponent(key)
    , target(target)
{
}

void ProjectileMovementComponent::onEntityFirstAddedToWorld(World*)
{
    Projectile* projectile = entity->as<Projectile>();
    if (!projectile)
    {
        LOG_WARN("ProjectileMovementComponent must be attached to a Projectile!");
        return;
    }

    // Calculate projectile lifetime based on speed
    const AttackDef* attackDef = projectile->getAttackDef();
    const MapNode pos = projectile->getPos();
    const int distance = MapUtils::getDistance(pos, target);
    const float speed = attackDef->projectileSpeed * 0.001f;  // TMP speed modifier
    lifetime = static_cast<int>(distance / speed);
}

void ProjectileMovementComponent::update(int delta)
{
    timeElapsed += delta;
    if (timeElapsed >= lifetime)
    {
        onProjectileArrived();
        return;
    }

    entity->moved = true;
}

void ProjectileMovementComponent::onProjectileArrived()
{
    // TODO: damage target (if present)
    entity->markForDeletion();
}

glm::vec2 ProjectileMovementComponent::getRenderOffset(int delta) const
{
    // TODO: We never actually move the projectile, we just simulate the movement with a render offset.
    // This may cause problems later down the line with fog of war.

    // Calculate the start and end position, in pixels.
    // This doesn't have to be precise, as long as the distances are correct; we only need the render offset relative to
    // the start position.
    const MapNode pos = entity->getPos();
    const glm::vec2 start = { //
        static_cast<float>(RenderUtils::tileToPx_X(pos.x)),
        static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y))
    };
    const glm::vec2 end = { //
        static_cast<float>(RenderUtils::tileToPx_X(target.x)),
        static_cast<float>(RenderUtils::tileToPx_Y(target.x, target.y))
    };

    const glm::vec2 journey = end - start;
    const float timeElapsedWithDelta = static_cast<float>(timeElapsed + delta);
    const float progress = MathUtils::clampf(timeElapsedWithDelta / lifetime, 0.f, 1.f);

    return journey * progress;
}

}  // namespace Rival
