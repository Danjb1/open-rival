#include "entity/components/UnitAnimationComponent.h"

#include <stdexcept>

#include "application/Resources.h"
#include "entity/Entity.h"
#include "entity/components/AnimationListener.h"
#include "entity/components/SpriteComponent.h"
#include "game/UnitDef.h"
#include "game/UnitType.h"
#include "game/World.h"
#include "utils/LogUtils.h"
#include "utils/TimeUtils.h"

namespace Rival {

const std::string UnitAnimationComponent::key = "unitAnimation";

UnitAnimationComponent::UnitAnimationComponent(const AnimationContainer<UnitAnimationType>& animContainer)
    : EntityComponent(key)
    , animContainer(animContainer)
    , animation(nullptr)
{
}

void UnitAnimationComponent::onEntityFirstAddedToWorld(World*)
{
    Unit* unit = entity->as<Unit>();
    if (!unit)
    {
        LOG_WARN("UnitAnimationComponent must be attached to a Unit!");
        return;
    }

    weakSpriteComponent = entity->requireComponentWeak<SpriteComponent>(SpriteComponent::key);

    weakFacingComponent = entity->getComponentWeak<FacingComponent>(FacingComponent::key);
    if (auto facingComponent = weakFacingComponent.lock())
    {
        facingComponent->setListener(this);
    }

    unit->addStateListener(this);

    // Determine animation based on initial state
    onUnitStateChanged(unit->getState());
}

void UnitAnimationComponent::destroy()
{
    if (auto facingComponent = weakFacingComponent.lock())
    {
        facingComponent->setListener(nullptr);
    }

    if (Unit* unit = entity->as<Unit>())
    {
        unit->removeStateListener(this);
    }
}

void UnitAnimationComponent::update(int delta)
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

void UnitAnimationComponent::onUnitStateChanged(const UnitState newState)
{
    if (newState == UnitState::Idle || newState == UnitState::WaitingToMove)
    {
        setAnimation(UnitAnimationType::Standing);
    }
    else if (newState == UnitState::Moving)
    {
        // TODO: Peasants may need to play the MovingWithBag animation
        setAnimation(UnitAnimationType::Moving);
    }
    else if (newState == UnitState::Attacking)
    {
        setAnimation(UnitAnimationType::Attacking);
    }
    else if (newState == UnitState::Dying)
    {
        setAnimation(UnitAnimationType::Dying);
    }
}

void UnitAnimationComponent::facingChanged(Facing)
{
    refreshSpriteComponent();
}

void UnitAnimationComponent::addListener(std::weak_ptr<AnimationListener<UnitAnimationType>> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.emplace(listener);
}

void UnitAnimationComponent::removeListener(std::weak_ptr<AnimationListener<UnitAnimationType>> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.erase(listener);
}

void UnitAnimationComponent::setAnimation(UnitAnimationType animType)
{
    if (animType == currentAnimType)
    {
        // No change
        return;
    }

    const Animation* newAnimation = animContainer.getAnimation(animType);
    animation = newAnimation;
    currentAnimType = animType;
    msPassedCurrentAnimFrame = 0;
    setCurrentAnimFrame(0);

    const int numAnimFrames = getNumAnimFrames();
    if (numAnimFrames == 1)
    {
        // A single-frame animation is finished immediately!
        CollectionUtils::forEachWeakPtr<AnimationListener<UnitAnimationType>>(
                listeners, [&](auto listener) { listener->onAnimationFinished(currentAnimType); });
    }
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
    const int prevAnimFrame = currentAnimFrame;
    const int newAnimFrame = (currentAnimFrame + 1) % numAnimFrames;

    setCurrentAnimFrame(newAnimFrame);
    msPassedCurrentAnimFrame -= msPerAnimFrame;

    if (newAnimFrame < prevAnimFrame)
    {
        // TODO: Don't bother emitting events unless they're needed (e.g. attacks)
        CollectionUtils::forEachWeakPtr<AnimationListener<UnitAnimationType>>(
                listeners, [&](auto listener) { listener->onAnimationFinished(currentAnimType); });
    }
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

    const bool isDeathAnim = (currentAnimType == UnitAnimationType::Dying);
    const int facingStride = getFacingStride();
    const int stride = facingStride > 0 ? facingStride : getNumAnimFrames();
    const int facingIndex = isDeathAnim ? facingComponent->getDeathFacingIndex() : facingComponent->getFacingIndex();

    return facingIndex * stride;
}

bool UnitAnimationComponent::hasAnimation(UnitAnimationType animType) const
{
    return animContainer.getAnimation(animType) != nullptr;
}

}  // namespace Rival
