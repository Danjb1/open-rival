#include "entity/Unit.h"

#include "entity/Entity.h"
#include "entity/components/MovementComponent.h"
#include "game/MapUtils.h"
#include "game/World.h"

namespace Rival {

void Unit::onReady()
{
    weakMovementComponent = requireComponentWeak<MovementComponent>(MovementComponent::key);
    if (auto movementComponent = weakMovementComponent.lock())
    {
        movementComponent->addListener(getWeakThis());
    }
}

void Unit::onDestroy()
{
    if (auto movementComponent = weakMovementComponent.lock())
    {
        movementComponent->removeListener(getWeakThis());
    }
}

void Unit::onUnitMoveStart(const MapNode*)
{
    setState(UnitState::Moving);
}

void Unit::onUnitPaused()
{
    setState(UnitState::Idle);
}

void Unit::onUnitStopped()
{
    setState(UnitState::Idle);
}

void Unit::addStateListener(UnitStateListener* listener)
{
    if (!listener)
    {
        return;
    }
    stateListeners.emplace(listener);
}

void Unit::removeStateListener(UnitStateListener* listener)
{
    if (!listener)
    {
        return;
    }
    stateListeners.erase(listener);
}

void Unit::setState(UnitState newState)
{
    if (state == newState)
    {
        // No change
        return;
    }

    state = newState;

    for (UnitStateListener* listener : stateListeners)
    {
        listener->onUnitStateChanged(newState);
    }
}

std::weak_ptr<Unit> Unit::getWeakThis()
{
    return std::dynamic_pointer_cast<Unit>(shared_from_this());
}

}  // namespace Rival
