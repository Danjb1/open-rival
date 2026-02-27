#include "entity/Unit.h"

#include "entity/Effect.h"
#include "entity/Entity.h"
#include "entity/components/AttackComponent.h"
#include "entity/components/DeathEffectComponent.h"
#include "entity/components/FlyerComponent.h"
#include "entity/components/MovementComponent.h"
#include "entity/components/SpriteComponent.h"
#include "entity/components/UnitAnimationComponent.h"
#include "game/MapUtils.h"
#include "game/World.h"
#include "gfx/RenderUtils.h"

namespace Rival {

void Unit::onReady()
{
    movementComponent = requireComponentShared<MovementComponent>(MovementComponent::key);
    movementComponent->addListener(getWeakThis());

    attackComponent = getComponentShared<AttackComponent>(AttackComponent::key);
    if (attackComponent)
    {
        attackComponent->addListener(getWeakThis());
    }

    healthComponent = requireComponentShared<HealthComponent>(HealthComponent::key);
    if (healthComponent)
    {
        healthComponent->addListener(getWeakThis());
    }

    facingComponent = getComponentShared<FacingComponent>(FacingComponent::key);
    spriteComponent = getComponentShared<SpriteComponent>(SpriteComponent::key);
    animationComponent = getComponentShared<UnitAnimationComponent>(UnitAnimationComponent::key);
    deathEffectComponent = getComponentShared<DeathEffectComponent>(DeathEffectComponent::key);
}

void Unit::onDestroy()
{
    movementComponent->removeListener(getWeakThis());

    if (attackComponent)
    {
        attackComponent->removeListener(getWeakThis());
    }

    if (healthComponent)
    {
        healthComponent->removeListener(getWeakThis());
    }

    if (animationComponent)
    {
        animationComponent->removeListener(getWeakThis());
    }
}

float Unit::getZOffset() const
{
    return movementComponent->getMovementMode() == MovementMode::Flying ? RenderUtils::zOffsetFlyers : 0.0f;
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

void Unit::onHealthChanged(Entity* /*entity*/, int /*prevValue*/, int /*newValue*/)
{
    // Nothing to do
}

void Unit::onMaxHealthChanged(Entity* /*entity*/, int /*prevValue*/, int /*newValue*/)
{
    // Nothing to do
}

void Unit::onHealthDepleted(Entity* /*entity*/)
{
    bool hasDeathAnim = false;

    if (animationComponent && animationComponent->hasAnimation(UnitAnimationType::Dying))
    {
        // If the Unit has a death animation, we await a callback to `onAnimationFinished`
        animationComponent->addListener(getWeakThis());
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
        movementComponent->requestStop();
        break;
    case UnitState::Attacking:
        if (attackComponent)
        {
            attackComponent->clearTarget();
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

    if (deathEffectComponent)
    {
        // We have to wait until the death effect finishes before triggering the death event
        Facing facing = Facing::North;
        if (facingComponent)
        {
            facing = facingComponent->getDeathFacing();
        }
        std::shared_ptr<Entity> deathEffect = deathEffectComponent->spawnEffect(facing, offsetY);
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

bool Unit::isFlying() const
{
    return std::dynamic_pointer_cast<FlyerComponent>(movementComponent) != nullptr;
}

}  // namespace Rival
