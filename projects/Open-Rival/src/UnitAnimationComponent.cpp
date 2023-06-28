#include "UnitAnimationComponent.h"

#include <stdexcept>

#include "Animations.h"
#include "Entity.h"
#include "Resources.h"
#include "SpriteComponent.h"
#include "TimeUtils.h"
#include "Unit.h"
#include "UnitDef.h"
#include "World.h"

namespace Rival {

const std::string UnitAnimationComponent::key = "unitAnimation";

UnitAnimationComponent::UnitAnimationComponent(const UnitDef& unitDef)
    : EntityComponent(key)
    , unitDef(unitDef)
    , animation(nullptr)
{
}

void UnitAnimationComponent::onEntitySpawned(World*)
{
    weakSpriteComponent = entity->requireComponentWeak<SpriteComponent>(SpriteComponent::key);

    weakFacingComponent = entity->getComponentWeak<FacingComponent>(FacingComponent::key);
    if (auto facingComponent = weakFacingComponent.lock())
    {
        facingComponent->setListener(this);
    }

    weakUnitPropsComponent = entity->getComponentWeak<UnitPropsComponent>(UnitPropsComponent::key);
    if (auto unitPropsComponent = weakUnitPropsComponent.lock())
    {
        unitPropsComponent->addStateListener(this);

        // Determine animation based on initial state
        onUnitStateChanged(unitPropsComponent->getState());
    }
}

void UnitAnimationComponent::onDelete()
{
    if (auto facingComponent = weakFacingComponent.lock())
    {
        facingComponent->setListener(nullptr);
    }
}

void UnitAnimationComponent::update()
{
    if (!animation)
    {
        return;
    }

    int numAnimFrames = getNumAnimFrames();

    if (numAnimFrames == 1)
    {
        // Nothing to animate
        return;
    }

    msPassedCurrentAnimFrame += TimeUtils::timeStepMs;

    int msPerAnimFrame = getMsPerAnimFrame();
    if (msPassedCurrentAnimFrame >= msPerAnimFrame)
    {
        advanceFrame(numAnimFrames, msPerAnimFrame);
    }
}

void UnitAnimationComponent::onUnitStateChanged(const UnitState newState)
{
    auto unitPropsComponent = weakUnitPropsComponent.lock();
    if (!unitPropsComponent)
    {
        return;
    }

    if (newState == UnitState::Idle)
    {
        setAnimation(unitDef.getAnimation(UnitAnimationType::Standing));
    }
    else if (newState == UnitState::Moving)
    {
        // TODO: Peasants may need to play the MovingWithBag animation
        setAnimation(unitDef.getAnimation(UnitAnimationType::Moving));
    }
}

void UnitAnimationComponent::facingChanged(Facing)
{
    refreshSpriteComponent();
}

void UnitAnimationComponent::setAnimation(const Animation* newAnimation)
{
    animation = newAnimation;
    msPassedCurrentAnimFrame = 0;
    setCurrentAnimFrame(0);
}

void UnitAnimationComponent::setCurrentAnimFrame(int newAnimFrame)
{
    currentAnimFrame = newAnimFrame;
    refreshSpriteComponent();
}

void UnitAnimationComponent::refreshSpriteComponent() const
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

void UnitAnimationComponent::advanceFrame(int numAnimFrames, int msPerAnimFrame)
{
    int newAnimFrame = (currentAnimFrame + 1) % numAnimFrames;
    setCurrentAnimFrame(newAnimFrame);
    msPassedCurrentAnimFrame -= msPerAnimFrame;
}

int UnitAnimationComponent::getCurrentSpriteIndex() const
{
    if (!animation)
    {
        return 0;
    }

    return animation->startIndex + getFacingOffset() + currentAnimFrame;
}

int UnitAnimationComponent::getNumAnimFrames() const
{
    if (!animation)
    {
        return 0;
    }

    return animation->endIndex - animation->startIndex + 1;
}

int UnitAnimationComponent::getMsPerAnimFrame() const
{
    // TODO: This should vary based on a Unit's speed
    return animation ? animation->msPerFrame : 0;
}

int UnitAnimationComponent::getFacingStride() const
{
    return animation ? animation->facingStride : 0;
}

int UnitAnimationComponent::getFacingOffset() const
{
    auto facingComponent = weakFacingComponent.lock();
    if (!facingComponent)
    {
        return 0;
    }

    int facingStride = getFacingStride();
    int stride = facingStride > 0 ? facingStride : getNumAnimFrames();
    int facingIndex = static_cast<int>(facingComponent->getFacing()) - static_cast<int>(Facing::South);
    return facingIndex * stride;
}

}  // namespace Rival
