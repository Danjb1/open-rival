#include "entity/Unit.h"

#include "entity/Entity.h"
#include "entity/components/AttackComponent.h"
#include "entity/components/MovementComponent.h"
#include "entity/components/UnitAnimationComponent.h"
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

    weakAttackComponent = getComponentWeak<AttackComponent>(AttackComponent::key);
    if (auto attackComponent = weakAttackComponent.lock())
    {
        attackComponent->addListener(getWeakThis());
    }

    weakHealthComponent = requireComponentWeak<HealthComponent>(HealthComponent::key);
    if (auto healthComponent = weakHealthComponent.lock())
    {
        healthComponent->addListener(getWeakThis());
    }

    weakAnimationComp = getComponentWeak<UnitAnimationComponent>(UnitAnimationComponent::key);
}

void Unit::onDestroy()
{
    if (auto movementComponent = weakMovementComponent.lock())
    {
        movementComponent->removeListener(getWeakThis());
    }

    if (auto attackComponent = weakAttackComponent.lock())
    {
        attackComponent->removeListener(getWeakThis());
    }

    if (auto healthComponent = weakHealthComponent.lock())
    {
        healthComponent->removeListener(getWeakThis());
    }

    if (auto animationComp = weakAnimationComp.lock())
    {
        animationComp->removeListener(getWeakThis());
    }
}

void Unit::onUnitMoveStart(const MapNode*)
{
    setState(UnitState::Moving);
}

void Unit::onUnitPaused()
{
    setState(UnitState::WaitingToMove);
}

void Unit::onUnitStopped()
{
    setState(UnitState::Idle);
}

void Unit::onAttackStarted()
{
    setState(UnitState::Attacking);
}

void Unit::onAttackFinished()
{
    setState(UnitState::Idle);
}

void Unit::onHealthChanged(int /*prevValue*/, int /*newValue*/)
{
    // Nothing to do
}

void Unit::onMaxHealthChanged(int /*prevValue*/, int /*newValue*/)
{
    // Nothing to do
}

void Unit::onHealthDepleted()
{
    if (auto animationComp = weakAnimationComp.lock())
    {
        // We delete the unit in the onAnimationFinished callback
        animationComp->addListener(getWeakThis());
    }
    else
    {
        markForDeletion();
    }

    setState(UnitState::Dying);
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

bool Unit::isBusy() const
{
    return state != UnitState::Idle && state != UnitState::WaitingToMove;
}

void Unit::abortAction()
{
    switch (state)
    {
    case UnitState::Moving:
    case UnitState::WaitingToMove:
        if (auto movementComponent = weakMovementComponent.lock())
        {
            movementComponent->requestStop();
        }
        break;
    case UnitState::Attacking:
        if (auto attackComponent = weakAttackComponent.lock())
        {
            attackComponent->setTarget({});
        }
        break;
    }
}

void Unit::onAnimationFinished(UnitAnimationType animType)
{
    if (animType == UnitAnimationType::Dying)
    {
        markForDeletion();
    }
}

}  // namespace Rival
