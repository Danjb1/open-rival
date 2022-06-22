#include "pch.h"

#include "FacingComponent.h"

#include "Entity.h"
#include "MapUtils.h"

namespace Rival {

const std::string FacingComponent::key = "facing";

FacingComponent::FacingComponent(Facing initialFacing)
    : EntityComponent(key)
    , facing(initialFacing)
{
}

void FacingComponent::onEntitySpawned(World*)
{
    weakMovementComponent = entity->requireComponentWeak<MovementComponent>(MovementComponent::key);
    if (auto movementComponent = weakMovementComponent.lock())
    {
        movementComponent->addListener(this);
    }
}

void FacingComponent::onDelete()
{
    if (auto movementComponent = weakMovementComponent.lock())
    {
        movementComponent->removeListener(this);
    }
}

void FacingComponent::onUnitMoveStart(const MapNode* nextNode)
{
    Facing newFacing = MapUtils::getDir(entity->getPos(), *nextNode);
    setFacing(newFacing);
}

void FacingComponent::onUnitJourneyEnd()
{
    // Do nothing
}

void FacingComponent::setListener(FacingListener* newListener)
{
    listener = newListener;
}

void FacingComponent::setFacing(Facing newFacing)
{
    facing = newFacing;
    notifyListener();
}

void FacingComponent::rotateLeft()
{
    int newFacing = (static_cast<int>(facing) - 1) % numFacings;
    facing = static_cast<Facing>(newFacing);
}

void FacingComponent::rotateRight()
{
    int newFacing = (static_cast<int>(facing) + 1) % numFacings;
    facing = static_cast<Facing>(newFacing);
}

void FacingComponent::notifyListener() const
{
    if (listener)
    {
        listener->facingChanged(facing);
    }
}

}  // namespace Rival
