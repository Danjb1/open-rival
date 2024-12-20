#include "entity/components/FacingComponent.h"

#include "entity/Entity.h"
#include "entity/components/MovementComponent.h"
#include "game/MapUtils.h"
#include "utils/EnumUtils.h"

namespace Rival {

const std::string FacingComponent::key = "facing";

FacingComponent::FacingComponent(Facing initialFacing)
    : EntityComponent(key)
    , facing(initialFacing)
{
}

void FacingComponent::onEntityFirstAddedToWorld(World*)
{
    // Some entities (e.g. projectiles) do not have a MovementComponent
    weakMovementComponent = entity->getComponentWeak<MovementComponent>(MovementComponent::key);
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

int FacingComponent::getDeathFacingIndex() const
{
    /*
     * Death animations only support 4 different facings,
     * so dying units need to "snap" to one of these directions.
     *
     * Return values:
     *   0 = south-west
     *   1 = north-west
     *   2 = north-east
     *   3 = south-east
     */
    switch (facing)
    {
    case Facing::South:
    case Facing::SouthWest:
        return 0;
    case Facing::West:
    case Facing::NorthWest:
        return 1;
    case Facing::North:
    case Facing::NorthEast:
        return 2;
    case Facing::East:
    case Facing::SouthEast:
        return 3;
    }

    throw std::runtime_error("Unsupported facing: " + std::to_string(EnumUtils::toIntegral(facing)));
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
