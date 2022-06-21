#include "pch.h"

#include "MovementComponent.h"

#include "Entity.h"
#include "TimerUtils.h"
#include "World.h"

namespace Rival {

const std::string MovementComponent::key = "movement";

MovementComponent::MovementComponent(Pathfinding::PassabilityChecker* passabilityChecker)
    : EntityComponent(key)
    , movement({ 0, 0 })
    , passabilityChecker(passabilityChecker)
{
}

void MovementComponent::update()
{
    if (route.isEmpty())
    {
        return;
    }

    entity->moved = true;

    // Update movement
    if (movement.timeElapsed >= movement.timeRequired)
    {
        completeMovement();
    }
    else
    {
        movement.timeElapsed += TimerUtils::timeStepMs;
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
    auto newRoute = Pathfinding::findPath(entity->getPos(), node, *entity->getWorld(), *passabilityChecker);
    setRoute(newRoute);
}

void MovementComponent::setRoute(Pathfinding::Route newRoute)
{
    route = newRoute;

    if (!route.isEmpty())
    {
        prepareNextMovement();
    }
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

    movement.timeRequired = ticksPerMove * TimerUtils::timeStepMs;

    for (MovementListener* listener : listeners)
    {
        listener->onUnitMoveStart(route.peek());
    }
}

/**
 * Called after moving to a new tile.
 */
void MovementComponent::completeMovement()
{
    MapNode node = route.pop();
    entity->setPos(node);

    movement.timeElapsed = 0;

    if (route.isEmpty())
    {
        for (MovementListener* listener : listeners)
        {
            listener->onUnitJourneyEnd();
        }
    }
    else
    {
        prepareNextMovement();
    }
}

}  // namespace Rival
