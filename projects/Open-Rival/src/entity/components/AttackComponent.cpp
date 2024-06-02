#include "entity/components/AttackComponent.h"

#include "entity/Entity.h"
#include "entity/Unit.h"
#include "entity/components/AttackListener.h"
#include "entity/components/FacingComponent.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/MovementComponent.h"
#include "game/World.h"

namespace Rival {

const std::string AttackComponent::key = "attack";

AttackComponent::AttackComponent()
    : EntityComponent(key)
{
}

void AttackComponent::onEntityAddedToWorld(World*)
{
    weakMovementComp = entity->getComponentWeak<MovementComponent>(MovementComponent::key);
    weakFacingComp = entity->getComponentWeak<FacingComponent>(FacingComponent::key);
    weakAnimationComp = entity->getComponentWeak<UnitAnimationComponent>(UnitAnimationComponent::key);

    if (auto animationComp = weakAnimationComp.lock())
    {
        std::weak_ptr<AttackComponent> weakThis = entity->getComponentWeak<AttackComponent>(key);
        animationComp->addListener(weakThis);
    }
}

void AttackComponent::destroy()
{
    if (auto animationComp = weakAnimationComp.lock())
    {
        std::weak_ptr<AttackComponent> weakThis = entity->getComponentWeak<AttackComponent>(key);
        animationComp->removeListener(weakThis);
    }
}

void AttackComponent::onAnimationFinished(UnitAnimationType animType)
{
    if (animType == UnitAnimationType::Attacking)
    {
        deliverAttack();
        attackState = AttackState::Cooldown;
        CollectionUtils::forEachWeakPtr<AttackListener>(
                listeners, [&](auto listener) { listener->onAttackFinished(); });
    }
}

void AttackComponent::update(int delta)
{
    if (attackState == AttackState::WaitingForIdle)
    {
        Unit* unit = entity->as<Unit>();
        if (unit->isBusy())
        {
            return;
        }
        attackState = AttackState::None;
    }

    if (attackState == AttackState::Attacking)
    {
        // Waiting for anim notify
        return;
    }

    if (attackState == AttackState::Cooldown)
    {
        updateCooldown(delta);
        return;
    }

    std::shared_ptr<Entity> targetEntity = weakTargetEntity.lock();
    if (!targetEntity)
    {
        // No valid target!
        return;
    }

    if (isInRange(targetEntity->getPos()))
    {
        requestAttack(targetEntity);
    }
    else
    {
        // TODO: We could call prepareToMove in some kind of "earlyUpdate" method to prevent obstructions between units
        // that all start moving at the same time
        moveToTarget(targetEntity->getPos());
    }
}

void AttackComponent::deliverAttack()
{
    std::shared_ptr<Entity> targetEntity = weakTargetEntity.lock();
    if (!targetEntity)
    {
        // Target no longer exists!
        return;
    }

    HealthComponent* healthComp = targetEntity->getComponent<HealthComponent>();
    healthComp->addHealth(-10);  // TMP
}

void AttackComponent::switchToNewTarget()
{
    weakTargetEntity = weakRequestedTargetEntity;
}

void AttackComponent::updateCooldown(int delta)
{
    cooldownTimeElapsed += delta;
    if (cooldownTimeElapsed >= cooldownDuration)
    {
        cooldownTimeElapsed = 0;
        attackState = AttackState::None;
        switchToNewTarget();
    }
}

bool AttackComponent::isInRange(const MapNode& node) const
{
    // TODO: Pick an attack
    const int range = 1;  // TMP
    const int distToTarget = MapUtils::getDistance(entity->getPos(), node);
    return distToTarget <= range;
}

void AttackComponent::requestAttack(std::shared_ptr<Entity> targetEntity)
{
    Unit* unit = entity->as<Unit>();
    if (unit->isBusy())
    {
        // Don't interrupt another action
        unit->abortAction();
        attackState = AttackState::WaitingForIdle;
    }
    else
    {
        startAttack(targetEntity);
    }
}

void AttackComponent::startAttack(std::shared_ptr<Entity> targetEntity)
{
    // Face the target
    if (auto facingComponent = weakFacingComp.lock())
    {
        const Facing newFacing = MapUtils::getDir(entity->getPos(), targetEntity->getPos());
        facingComponent->setFacing(newFacing);
    }

    CollectionUtils::forEachWeakPtr<AttackListener>(listeners, [&](auto listener) { listener->onAttackStarted(); });
    attackState = AttackState::Attacking;
}

void AttackComponent::moveToTarget(const MapNode& node)
{
    auto moveComponent = weakMovementComp.lock();
    if (!moveComponent)
    {
        // We are not capable of movement
        return;
    }

    if (moveComponent->getRoute().getIntendedDestination() == node)
    {
        // We are already moving towards the target
        return;
    }

    // TODO: Avoid repathing every tick if the target is unreachable
    Pathfinding::Context context;
    Pathfinding::Hints hints;
    // We wants units to make every effort to reach the destination;
    // this forces them to pathfind reasonably far around obstructions.
    // TODO: This might not be enough, still.
    hints.obstructedMovementCost = 8;
    moveComponent->moveTo(node, context, hints);
}

void AttackComponent::setTarget(std::weak_ptr<Entity> weakNewTarget)
{
    std::shared_ptr<Entity> newTarget = weakNewTarget.lock();
    if (newTarget && newTarget->getId() == entity->getId())
    {
        // Can't attack self
        return;
    }

    weakRequestedTargetEntity = weakNewTarget;

    if (attackState == AttackState::None)
    {
        switchToNewTarget();
    }
}

void AttackComponent::addListener(std::weak_ptr<AttackListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.emplace(listener);
}

void AttackComponent::removeListener(std::weak_ptr<AttackListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.erase(listener);
}

}  // namespace Rival
