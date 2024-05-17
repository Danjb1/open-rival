#include "entity/components/FacingComponent.h"

#include "entity/Entity.h"
#include "entity/components/MovementComponent.h"
#include "game/MapUtils.h"

namespace Rival {

const std::string FacingComponent::key = "facing";

FacingComponent::FacingComponent(Facing initialFacing)
    : EntityComponent(key)
    , facing(initialFacing)
{
}

void FacingComponent::onEntityAddedToWorld(World*)
{
    weakMovementComponent = entity->requireComponentWeak<MovementComponent>(MovementComponent::key);
    if (auto movementComponent = weakMovementComponent.lock())
    {
        std::weak_ptr<FacingComponent> weakThis = entity->getComponentWeak<FacingComponent>(key);
        movementComponent->addListener(weakThis);
    }
}

void FacingComponent::destroy()
{
    if (auto movementComponent = weakMovementComponent.lock())
    {
        std::weak_ptr<FacingComponent> weakThis = entity->getComponentWeak<FacingComponent>(key);
        movementComponent->removeListener(weakThis);
    }
}

void FacingComponent::onUnitMoveStart(const MapNode* nextNode)
{
    Facing newFacing = MapUtils::getDir(entity->getPos(), *nextNode);
    setFacing(newFacing);
}

void FacingComponent::onUnitPaused()
{
    // Do nothing
}

void FacingComponent::onUnitStopped()
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
