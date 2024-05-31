#include "entity/components/AttackComponent.h"

#include "entity/Entity.h"
#include "entity/components/FacingComponent.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/MovementComponent.h"
#include "game/World.h"

namespace Rival {

const std::string AttackComponent::key = "attack";

bool AttackInstance::isFinished() const
{
    return timeElapsed >= duration;
}

void AttackInstance::reset()
{
    timeElapsed = 0;
}

AttackComponent::AttackComponent()
    : EntityComponent(key)
{
}

void AttackComponent::onEntityAddedToWorld(World*)
{
    weakMovementComp = entity->getComponentWeak<MovementComponent>(MovementComponent::key);
    if (auto movementComponent = weakMovementComp.lock())
    {
        std::weak_ptr<AttackComponent> weakThis = entity->getComponentWeak<AttackComponent>(key);
        movementComponent->addListener(weakThis);
    }

    weakFacingComp = entity->getComponentWeak<FacingComponent>(FacingComponent::key);
}

void AttackComponent::destroy()
{
    if (auto movementComponent = weakMovementComp.lock())
    {
        std::weak_ptr<AttackComponent> weakThis = entity->getComponentWeak<AttackComponent>(key);
        movementComponent->removeListener(weakThis);
    }
}

void AttackComponent::onUnitMoveStart(const MapNode*)
{
    // Do nothing
}

void AttackComponent::onUnitPaused()
{
    // Do nothing
}

void AttackComponent::onUnitStopped()
{
    if (attackState == AttackState::WaitingForMovement)
    {
        attackState = AttackState::None;
    }
}

void AttackComponent::update(int delta)
{
    if (attackState == AttackState::Attacking)
    {
        updateAttack(delta);
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

void AttackComponent::updateAttack(int delta)
{
    attackInstance.timeElapsed += delta;

    if (attackInstance.isFinished())
    {
        deliverAttack();
        attackState = AttackState::Cooldown;
        attackInstance.reset();
        CollectionUtils::forEachWeakPtr<AttackListener>(
                listeners, [&](auto listener) { listener->onAttackFinished(); });
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
    if (attackState != AttackState::None)
    {
        return;
    }

    auto moveComponent = weakMovementComp.lock();
    if (moveComponent && moveComponent->isCurrentlyMoving())
    {
        moveComponent->requestStop();
        attackState = AttackState::WaitingForMovement;
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

    // TODO: Get this data from the current attack ability / animation
    attackInstance.duration = 600;
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
    moveComponent->moveTo(node, context);
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
