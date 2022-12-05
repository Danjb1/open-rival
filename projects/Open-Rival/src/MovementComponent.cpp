#include "pch.h"

#include "MovementComponent.h"

#include "Entity.h"
#include "TimerUtils.h"
#include "World.h"

namespace Rival {

const std::string MovementComponent::key = "movement";

void Movement::clear()
{
    destination = { -1, -1 };
    timeElapsed = 0;
    timeRequired = 0;
}

bool Movement::isValid() const
{
    return timeRequired > 0;
}

bool Movement::isInProgress() const
{
    return isValid() && !isFinished();
}

bool Movement::isFinished() const
{
    return isValid() && timeElapsed >= timeRequired;
}

MovementComponent::MovementComponent(
        const Pathfinding::PassabilityChecker& passabilityChecker, Pathfinding::PassabilityUpdater& passabilityUpdater)
    : EntityComponent(key)
    , movement({ 0, 0 })
    , passabilityChecker(passabilityChecker)
    , passabilityUpdater(passabilityUpdater)
{
}

void MovementComponent::update()
{
    // Prepare the next movement if we are not currently moving between tiles
    if (!movement.isValid())
    {
        if (!prepareNextMovement())
        {
            return;
        }
    }

    updateMovement();
    entity->moved = true;
}

void MovementComponent::addListener(MovementListener* listener)
{
    if (!listener)
    {
        return;
    }
    listeners.emplace(listener);
}

void MovementComponent::removeListener(MovementListener* listener)
{
    if (!listener)
    {
        return;
    }
    listeners.erase(listener);
}

void MovementComponent::moveTo(MapNode node)
{
    const MapNode startPos = getStartPosForNextMovement();
    auto newRoute = Pathfinding::findPath(startPos, node, *entity->getWorld(), passabilityChecker);
    setRoute(newRoute);
}

MapNode MovementComponent::getStartPosForNextMovement() const
{
    // If we are already moving between tiles, use the tile where we're about to end up
    return movement.isInProgress() ? movement.destination : entity->getPos();
}

void MovementComponent::setRoute(Pathfinding::Route newRoute)
{
    route = newRoute;
}

void MovementComponent::updateMovement()
{
    movement.timeElapsed += TimerUtils::timeStepMs;

    if (movement.isFinished())
    {
        completeMovement();
    }
}

/**
 * Called before moving to a new tile.
 */
bool MovementComponent::prepareNextMovement()
{
    // Check if we have a route planned
    if (route.isEmpty())
    {
        return false;
    }

    // Verify that the destination tile is traversable
    World* world = entity->getWorld();
    if (!passabilityChecker.isNodeTraversable(*world, *route.peek()))
    {
        // Destination tile is either temporarily or permanently blocked.
        // TODO: If we know that the tile will become available again (e.g. if a unit is leaving the tile), then wait.
        //   Otherwise, re-plan our route.
        onStop();
        return false;
    }

    // Configure the new movement
    movement.destination = route.pop();
    movement.timeRequired = ticksPerMove * TimerUtils::timeStepMs;

    // Horizontal movements should take longer because the distance is greater
    Facing facing = MapUtils::getDir(entity->getPos(), movement.destination);
    if (facing == Facing::East || facing == Facing::West)
    {
        movement.timeRequired = static_cast<int>(movement.timeRequired * horizontalMoveTimeMultiplier);
    }

    // Update tile passability
    passabilityUpdater.onUnitLeavingTile(*world, entity->getPos());
    passabilityUpdater.onUnitEnteringTile(*world, movement.destination);

    // Inform listeners
    for (MovementListener* listener : listeners)
    {
        listener->onUnitMoveStart(&movement.destination);
    }

    return true;
}

/**
 * Called after moving to a new tile.
 */
void MovementComponent::completeMovement()
{
    // Update tile passability
    World* world = entity->getWorld();
    passabilityUpdater.onUnitLeftTile(*world, entity->getPos());
    passabilityUpdater.onUnitEnteredTile(*world, movement.destination);

    // Update entity position
    // TODO: Set the unit's position to the new tile once they are more than halfway through the movement?
    entity->setPos(movement.destination);

    movement.clear();

    if (route.isEmpty())
    {
        // Reached end of route
        onStop();
    }
}

void MovementComponent::onStop()
{
    // Inform listeners
    for (MovementListener* listener : listeners)
    {
        listener->onUnitStopped();
    }
}

}  // namespace Rival
