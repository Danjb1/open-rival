#include "entity/components/ProjectileAnimationComponent.h"

#include <stdexcept>

#include "application/Resources.h"
#include "entity/Entity.h"
#include "entity/Projectile.h"
#include "entity/components/SpriteComponent.h"
#include "game/World.h"
#include "utils/LogUtils.h"
#include "utils/TimeUtils.h"

namespace Rival {

const std::string ProjectileAnimationComponent::key = "ProjectileAnimation";

ProjectileAnimationComponent::ProjectileAnimationComponent(const ProjectileDef& projectileDef)
    : EntityComponent(key)
    , animation(0, projectileDef.numAnimFrames - 1, msPerFrame, 0)
{
}

void ProjectileAnimationComponent::onEntityFirstAddedToWorld(World*)
{
    Projectile* projectile = entity->as<Projectile>();
    if (!projectile)
    {
        LOG_WARN("ProjectileAnimationComponent must be attached to a Projectile!");
        return;
    }

    weakSpriteComponent = entity->requireComponentWeak<SpriteComponent>(SpriteComponent::key);
    weakFacingComponent = entity->getComponentWeak<FacingComponent>(FacingComponent::key);

    setCurrentAnimFrame(0);
}

void ProjectileAnimationComponent::update(int delta)
{
    msPassedCurrentAnimFrame += delta;

    const int msPerAnimFrame = animation.msPerFrame;
    if (msPassedCurrentAnimFrame >= msPerAnimFrame)
    {
        const int numAnimFrames = getNumAnimFrames();
        advanceFrame(numAnimFrames, msPerAnimFrame);
    }
}

void ProjectileAnimationComponent::setCurrentAnimFrame(int newAnimFrame)
{
    currentAnimFrame = newAnimFrame;
    refreshSpriteComponent();
}

void ProjectileAnimationComponent::refreshSpriteComponent() const
{
    auto spriteComponent = weakSpriteComponent.lock();
    if (!spriteComponent)
    {
        return;
    }

    // Update the SpriteComponent, if present.
    // This is what actually causes the rendered image to change.
    spriteComponent->setTxIndex(getCurrentSpriteIndex());
}

void ProjectileAnimationComponent::advanceFrame(int numAnimFrames, int msPerAnimFrame)
{
    const int newAnimFrame = (currentAnimFrame + 1) % numAnimFrames;

    setCurrentAnimFrame(newAnimFrame);
    msPassedCurrentAnimFrame -= msPerAnimFrame;
}

int ProjectileAnimationComponent::getCurrentSpriteIndex() const
{
    return animation.startIndex + getFacingOffset() + currentAnimFrame;
}

int ProjectileAnimationComponent::getNumAnimFrames() const
{
    return animation.endIndex - animation.startIndex + 1;
}

int ProjectileAnimationComponent::getFacingOffset() const
{
    auto facingComponent = weakFacingComponent.lock();
    if (!facingComponent)
    {
        return 0;
    }

    const int facingStride = animation.facingStride;
    const int stride = facingStride > 0 ? facingStride : getNumAnimFrames();
    const int facingIndex = facingComponent->getFacingIndex();
    return facingIndex * stride;
}

}  // namespace Rival
