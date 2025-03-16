#include "entity/components/ProjectileAnimationComponent.h"

#include "entity/Projectile.h"
#include "utils/LogUtils.h"

namespace Rival {

const std::string ProjectileAnimationComponent::key = "ProjectileAnimation";

ProjectileAnimationComponent::ProjectileAnimationComponent(const ProjectileDef& projectileDef)
    : BasicAnimationComponent(key)
    , projectileAnimation(0, projectileDef.numAnimFrames - 1, msPerFrame, 0)
{
    animation = &projectileAnimation;
}

void ProjectileAnimationComponent::onEntityFirstAddedToWorld(World* world)
{
    Projectile* projectile = entity->as<Projectile>();
    if (!projectile)
    {
        LOG_WARN("ProjectileAnimationComponent must be attached to a Projectile!");
        return;
    }

    BasicAnimationComponent::onEntityFirstAddedToWorld(world);

    setCurrentAnimFrame(0);
}

}  // namespace Rival
