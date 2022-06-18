#include "pch.h"

#include "UnitPropsComponent.h"

#include "Entity.h"
#include "MapUtils.h"

namespace Rival {

const std::string UnitPropsComponent::key = "unit_props";

UnitPropsComponent::UnitPropsComponent(Unit::Type type)
    : EntityComponent(key)
    , type(type)
{
}

void UnitPropsComponent::onEntitySpawned(World*)
{
    weakMovementComponent = entity->requireComponentWeak<MovementComponent>(MovementComponent::key);
    if (auto movementComponent = weakMovementComponent.lock())
    {
        movementComponent->addListener(this);
    }
}

void UnitPropsComponent::onDelete()
{
    if (auto movementComponent = weakMovementComponent.lock())
    {
        movementComponent->removeListener(this);
    }
}

void UnitPropsComponent::onUnitMoveStart(const MapNode*)
{
    setState(UnitState::Moving);
}

void UnitPropsComponent::onUnitJourneyEnd()
{
    setState(UnitState::Idle);
}

void UnitPropsComponent::addStateListener(UnitStateListener* listener)
{
    if (!listener)
    {
        return;
    }
    stateListeners.emplace(listener);
}

void UnitPropsComponent::removeStateListener(UnitStateListener* listener)
{
    if (!listener)
    {
        return;
    }
    stateListeners.erase(listener);
}

void UnitPropsComponent::setState(UnitState newState)
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

}  // namespace Rival
