#include "entity/Unit.h"

#include "entity/Effect.h"
#include "entity/Entity.h"
#include "entity/components/AttackComponent.h"
#include "entity/components/DeathEffectComponent.h"
#include "entity/components/MovementComponent.h"
#include "entity/components/SpriteComponent.h"
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

    weakFacingComponent = getComponentWeak<FacingComponent>(FacingComponent::key);
    weakSpriteComponent = getComponentWeak<SpriteComponent>(SpriteComponent::key);
    weakAnimationComponent = getComponentWeak<UnitAnimationComponent>(UnitAnimationComponent::key);
    weakDeathEffectComponent = getComponentWeak<DeathEffectComponent>(DeathEffectComponent::key);
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

    if (auto animationComp = weakAnimationComponent.lock())
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
    bool hasDeathAnim = false;

    auto animationComp = weakAnimationComponent.lock();
    if (animationComp && animationComp->hasAnimation(UnitAnimationType::Dying))
    {
        // If the Unit has a death animation, we await a callback to `onAnimationFinished`
        animationComp->addListener(getWeakThis());
        hasDeathAnim = true;
    }

    setState(UnitState::Dying);

    if (!hasDeathAnim)
    {
        trySpawnDeathEffect();
    }
}

void Unit::addStateListener(std::weak_ptr<UnitStateListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    stateListeners.emplace(listener);
}

void Unit::removeStateListener(std::weak_ptr<UnitStateListener> listener)
{
    if (listener.expired())
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

    CollectionUtils::forEachWeakPtr<UnitStateListener>(
            stateListeners, [&](auto listener) { listener->onUnitStateChanged(newState); });
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
        trySpawnDeathEffect();
    }
}

void Unit::trySpawnDeathEffect()
{
    setVisible(false);

    if (auto deathEffectComponent = weakDeathEffectComponent.lock())
    {
        // We have to wait until the death effect finishes before triggering the death event
        Facing facing = Facing::North;
        if (auto facingComponent = weakFacingComponent.lock())
        {
            facing = facingComponent->getDeathFacing();
        }
        std::shared_ptr<Entity> deathEffect = deathEffectComponent->spawnEffect(facing);
        if (Effect* effect = deathEffect->as<Effect>())
        {
            effect->addListener(getWeakThis());
        }
    }
    else
    {
        triggerDeathEvent();
    }
}

void Unit::onEffectFinished(Effect*)
{
    // The death effect has just finished, now the Unit is well and truly dead
    triggerDeathEvent();
}

void Unit::triggerDeathEvent()
{
    // TODO: drop items
    markForDeletion();
}

}  // namespace Rival
