#include "entity/components/BasicAnimationComponent.h"

#include "application/Resources.h"
#include "entity/Entity.h"
#include "entity/components/AnimationListener.h"
#include "entity/components/SpriteComponent.h"
#include "game/World.h"
#include "utils/LogUtils.h"
#include "utils/TimeUtils.h"

namespace Rival {

BasicAnimationComponent::BasicAnimationComponent(const std::string& key)
    : EntityComponent(key)
{
}

void BasicAnimationComponent::onEntityFirstAddedToWorld(World*)
{
    weakSpriteComponent = entity->requireComponentWeak<SpriteComponent>(SpriteComponent::key);

    weakFacingComponent = entity->getComponentWeak<FacingComponent>(FacingComponent::key);
    if (auto facingComponent = weakFacingComponent.lock())
    {
        facingComponent->setListener(this);
    }
}

void BasicAnimationComponent::destroy()
{
    if (auto facingComponent = weakFacingComponent.lock())
    {
        facingComponent->setListener(nullptr);
    }
}

void BasicAnimationComponent::update(int delta)
{
    if (!animation)
    {
        return;
    }

    const int numAnimFrames = getNumAnimFrames();

    if (numAnimFrames == 1)
    {
        // Nothing to animate
        return;
    }

    msPassedCurrentAnimFrame += delta;

    const int msPerAnimFrame = getMsPerAnimFrame();
    if (msPassedCurrentAnimFrame >= msPerAnimFrame)
    {
        advanceFrame(numAnimFrames, msPerAnimFrame);
    }
}

void BasicAnimationComponent::facingChanged(Facing)
{
    refreshSpriteComponent();
}

void BasicAnimationComponent::setAnimationData(const Animation* newAnimation)
{
    if (newAnimation == animation)
    {
        // No change
        return;
    }

    animation = newAnimation;
    msPassedCurrentAnimFrame = 0;
    setCurrentAnimFrame(0);
}

void BasicAnimationComponent::setCurrentAnimFrame(int newAnimFrame)
{
    currentAnimFrame = newAnimFrame;
    refreshSpriteComponent();
}

void BasicAnimationComponent::refreshSpriteComponent() const
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

void BasicAnimationComponent::advanceFrame(int numAnimFrames, int msPerAnimFrame)
{
    const int prevAnimFrame = currentAnimFrame;
    const int newAnimFrame = (currentAnimFrame + 1) % numAnimFrames;

    setCurrentAnimFrame(newAnimFrame);
    msPassedCurrentAnimFrame -= msPerAnimFrame;

    if (newAnimFrame < prevAnimFrame)
    {
        animationFinished();
    }
}

void BasicAnimationComponent::animationFinished()
{
    // Do nothing by default
}

int BasicAnimationComponent::getCurrentSpriteIndex() const
{
    if (!animation)
    {
        return 0;
    }

    return animation->startIndex + getFacingOffset() + currentAnimFrame;
}

int BasicAnimationComponent::getNumAnimFrames() const
{
    if (!animation)
    {
        return 0;
    }

    return animation->endIndex - animation->startIndex + 1;
}

int BasicAnimationComponent::getMsPerAnimFrame() const
{
    return animation ? animation->msPerFrame : 0;
}

int BasicAnimationComponent::getFacingStride() const
{
    return animation ? animation->facingStride : 0;
}

int BasicAnimationComponent::getFacingOffset() const
{
    auto facingComponent = weakFacingComponent.lock();
    if (!facingComponent)
    {
        return 0;
    }

    const int facingStride = getFacingStride();
    const int stride = facingStride > 0 ? facingStride : getNumAnimFrames();
    const int facingIndex = getFacingIndex(facingComponent);

    return facingIndex * stride;
}

int BasicAnimationComponent::getFacingIndex(std::shared_ptr<FacingComponent> facingComponent) const
{
    return facingComponent->getFacingIndex();
}

}  // namespace Rival
