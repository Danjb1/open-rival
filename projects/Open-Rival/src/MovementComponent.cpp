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

MovementComponent::MovementComponent(Pathfinding::PassabilityChecker* passabilityChecker)
    : EntityComponent(key)
    , movement({ 0, 0 })
    , passabilityChecker(passabilityChecker)
{
}

void MovementComponent::update()
{
    if (route.isEmpty() && !movement.isValid())
    {
        return;
    }

    entity->moved = true;

    // Update movement
    if (movement.isFinished())
    {
        completeMovement();
    }
    else
    {
        updateMovement();
    }
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
    auto newRoute = Pathfinding::findPath(startPos, node, *entity->getWorld(), *passabilityChecker);
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

    if (!movement.isInProgress())
    {
        // Start the new route immediately
        prepareNextMovement();
    }
}

void MovementComponent::updateMovement()
{
    movement.timeElapsed += TimerUtils::timeStepMs;
}

/**
 * Called before moving to a new tile.
 */
void MovementComponent::prepareNextMovement()
{
    if (route.isEmpty())
    {
        return;
    }

    movement.destination = route.pop();
    movement.timeRequired = ticksPerMove * TimerUtils::timeStepMs;

    for (MovementListener* listener : listeners)
    {
        listener->onUnitMoveStart(&movement.destination);
    }
}

/**
 * Called after moving to a new tile.
 */
void MovementComponent::completeMovement()
{
    entity->setPos(movement.destination);

    movement.clear();

    if (route.isEmpty())
    {
        for (MovementListener* listener : listeners)
        {
            listener->onUnitStopped();
        }
    }
    else
    {
        prepareNextMovement();
    }
}

}  // namespace Rival
