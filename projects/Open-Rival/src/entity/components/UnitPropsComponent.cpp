#include "entity/components/UnitPropsComponent.h"

#include "entity/Entity.h"
#include "entity/components/MovementComponent.h"
#include "game/MapUtils.h"

namespace Rival {

const std::string UnitPropsComponent::key = "unit_props";

UnitPropsComponent::UnitPropsComponent(Unit::Type type, std::string name, bool isNameUnique)
    : EntityComponent(key)
    , type(type)
    , name(name)
    , isNameUnique(isNameUnique)
{
}

void UnitPropsComponent::onEntitySpawned(World*)
{
    weakMovementComponent = entity->requireComponentWeak<MovementComponent>(MovementComponent::key);
    if (auto movementComponent = weakMovementComponent.lock())
    {
        std::weak_ptr<UnitPropsComponent> weakThis = entity->getComponentWeak<UnitPropsComponent>(key);
        movementComponent->addListener(weakThis);
    }
}

void UnitPropsComponent::onDelete()
{
    if (auto movementComponent = weakMovementComponent.lock())
    {
        std::weak_ptr<UnitPropsComponent> weakThis = entity->getComponentWeak<UnitPropsComponent>(key);
        movementComponent->removeListener(weakThis);
    }
}

void UnitPropsComponent::onUnitMoveStart(const MapNode*)
{
    setState(UnitState::Moving);
}

void UnitPropsComponent::onUnitPaused()
{
    setState(UnitState::Idle);
}

void UnitPropsComponent::onUnitStopped()
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
