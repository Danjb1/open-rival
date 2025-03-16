#include "entity/components/UnitAnimationComponent.h"

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
    : BasicAnimationComponent(key)
    , animContainer(animContainer)
{
}

void UnitAnimationComponent::onEntityFirstAddedToWorld(World* world)
{
    Unit* unit = entity->as<Unit>();
    if (!unit)
    {
        LOG_WARN("UnitAnimationComponent must be attached to a Unit!");
        return;
    }

    BasicAnimationComponent::onEntityFirstAddedToWorld(world);

    unit->addStateListener(this);

    // Determine animation based on initial state
    onUnitStateChanged(unit->getState());
}

void UnitAnimationComponent::destroy()
{
    BasicAnimationComponent::destroy();

    if (Unit* unit = entity->as<Unit>())
    {
        unit->removeStateListener(this);
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

    currentAnimType = animType;
    const Animation* newAnimation = animContainer.getAnimation(animType);
    setAnimationData(newAnimation);

    const int numAnimFrames = getNumAnimFrames();
    if (numAnimFrames == 1)
    {
        // A single-frame animation is finished immediately!
        animationFinished();
    }
}

void UnitAnimationComponent::animationFinished()
{
    // TODO: Don't bother emitting events unless they're needed (e.g. attacks)
    CollectionUtils::forEachWeakPtr<AnimationListener<UnitAnimationType>>(
            listeners, [&](auto listener) { listener->onAnimationFinished(currentAnimType); });
}

int UnitAnimationComponent::getMsPerAnimFrame() const
{
    // TODO: This should vary based on a Unit's speed
    return BasicAnimationComponent::getMsPerAnimFrame();
}

int UnitAnimationComponent::getFacingIndex(std::shared_ptr<FacingComponent> facingComponent) const
{
    const bool isDeathAnim = (currentAnimType == UnitAnimationType::Dying);
    return isDeathAnim ? facingComponent->getDeathFacingIndex() : facingComponent->getFacingIndex();
}

bool UnitAnimationComponent::hasAnimation(UnitAnimationType animType) const
{
    return animContainer.getAnimation(animType) != nullptr;
}

}  // namespace Rival
