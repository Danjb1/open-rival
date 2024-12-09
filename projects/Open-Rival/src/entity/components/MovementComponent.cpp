#include "entity/components/MovementComponent.h"

#include "entity/Entity.h"
#include "entity/Unit.h"
#include "game/World.h"
#include "utils/LogUtils.h"
#include "utils/TimeUtils.h"

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

bool Movement::isPastHalfway() const
{
    return timeElapsed > timeRequired / 2;
}

bool Movement::isFinished() const
{
    return isValid() && timeElapsed >= timeRequired;
}

MovementComponent::MovementComponent(MovementMode movementMode,
        const Pathfinding::PassabilityChecker& passabilityChecker,
        Pathfinding::PassabilityUpdater& passabilityUpdater)
    : EntityComponent(key)
    , movementMode(movementMode)
    , movement({ 0, 0 })
    , passabilityChecker(passabilityChecker)
    , passabilityUpdater(passabilityUpdater)
{
}

void MovementComponent::onEntityFirstAddedToWorld(World*)
{
    Unit* unit = entity->as<Unit>();
    if (!unit)
    {
        LOG_WARN("MovementComponent must be attached to a Unit!");
        return;
    }

    unit->addStateListener(this);
}

void MovementComponent::onEntityRemovedFromWorld(World*)
{
    resetPassability(true);
}

void MovementComponent::destroy()
{
    if (Unit* unit = entity->as<Unit>())
    {
        unit->removeStateListener(this);
    }
}

void MovementComponent::onUnitStateChanged(const UnitState newState)
{
    if (route.isEmpty())
    {
        // Nothing to do!
        return;
    }

    if (newState == UnitState::Idle && wasAbortActionRequested)
    {
        // The action we aborted has just finished, so we can now start moving
        return;
    }

    if (newState != UnitState::Moving && newState != UnitState::WaitingToMove)
    {
        // It is possible that a route was planned but never started.
        // We need to clear this route when we start a new action otherwise the unit may start moving unexpectedly.
        route = {};
    }
}

void MovementComponent::update(int delta)
{
    if (wasAbortActionRequested)
    {
        Unit* unit = entity->as<Unit>();
        if (unit->isBusy())
        {
            return;
        }
        wasAbortActionRequested = false;
    }

    // Prepare the next movement if we are not currently moving between tiles
    if (!movement.isValid())
    {
        if (!tryStartNextMovement())
        {
            return;
        }
    }

    updateMovement(delta);
    entity->moved = true;
}

void MovementComponent::addListener(std::weak_ptr<MovementListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.emplace(listener);
}

void MovementComponent::removeListener(std::weak_ptr<MovementListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.erase(listener);
}

void MovementComponent::prepareForMovement()
{
    if (isCurrentlyMoving())
    {
        // If we're already moving then the passability is already set correctly
        return;
    }

    World* world = entity->getWorld();
    passabilityUpdater.onUnitPreparingMove(*world, entity->getPos());
}

void MovementComponent::resetPassability(bool wasRemoved)
{
    World* world = entity->getWorld();

    // If we were moving to another tile, we should unblock it
    if (movement.isValid() && entity->getPos() == movement.start)
    {
        passabilityUpdater.onUnitMoveAborted(*world, movement.destination);
    }

    if (wasRemoved)
    {
        // Reset the current tile's passability in case we were preparing to move out of it.
        // Normally the PassabilityComponent would reset our current tile's passability but this does not consider
        // passability flags relating to movement.
        passabilityUpdater.onUnitMoveAborted(*world, entity->getPos());
    }
    else
    {
        passabilityUpdater.onUnitStopped(*world, entity->getPos());
    }
}

bool MovementComponent::moveTo(const MapNode& node, Pathfinding::Context& context, Pathfinding::Hints hints)
{
    ticksSpentWaiting = 0;
    prepareForMovement();

    const MapNode startPos = getStartPosForNextMovement();
    cachedHints = hints;
    route = Pathfinding::findPath(startPos, node, *entity->getWorld(), passabilityChecker, context, hints);

    if (route.isEmpty())
    {
        // Failed to plan a route
        resetPassability(false);
        return false;
    }

    return true;
}

void MovementComponent::requestStop()
{
    if (route.isEmpty())
    {
        // Nothing to do!
        return;
    }

    // Clear the route to prevent further movement
    route = {};

    if (isCurrentlyMoving())
    {
        // Let the current movement finish naturally
        return;
    }

    stopMovement();
}

bool MovementComponent::isCurrentlyMoving() const
{
    return movement.isInProgress();
}

MapNode MovementComponent::getStartPosForNextMovement() const
{
    // If we are already moving between tiles, use the tile where we're about to end up
    return isCurrentlyMoving() ? movement.destination : entity->getPos();
}

void MovementComponent::updateMovement(int delta)
{
    const bool wasPastHalfway = movement.isPastHalfway();
    movement.timeElapsed += delta;

    if (movement.isFinished())
    {
        onCompletedMoveToNewTile();
    }
    else if (!wasPastHalfway && movement.isPastHalfway())
    {
        // Once the unit has passed the halfway point, they are considered to have moved tiles
        onLeftPreviousTile();
    }
}

/**
 * Called before moving to a new tile.
 */
bool MovementComponent::tryStartNextMovement()
{
    // Check if we have a route planned
    if (route.isEmpty())
    {
        return false;
    }

    // Check that the destination is still pathable
    World* world = entity->getWorld();
    const MapNode* nextNode = route.peek();
    if (!passabilityChecker.isNodePathable(*world, *nextNode))
    {
        // Tile is no longer pathable, e.g. a building has been placed or terrain has changed
        stopMovement();
        return false;
    }

    // Check that the destination is traversable
    if (!passabilityChecker.isNodeTraversable(*world, *nextNode))
    {
        if (!tryRepathAroundNextNode(*world))
        {
            return false;
        }
    }

    Unit* unit = entity->as<Unit>();
    if (unit->getState() != UnitState::Moving && unit->isBusy())
    {
        // Don't interrupt another action
        wasAbortActionRequested = true;
        unit->abortAction();
        return false;
    }

    startNextMovement(*world);
    return true;
}

void MovementComponent::startNextMovement(PathfindingMap& map)
{
    numFailedRepathAttempts = 0;
    ticksSpentWaiting = 0;

    // Configure the new movement
    movement.start = entity->getPos();
    movement.destination = route.pop();
    movement.timeRequired = ticksPerMove * TimeUtils::timeStepMs;

    // Horizontal movements should take longer because the distance is greater
    const Facing facing = MapUtils::getDir(entity->getPos(), movement.destination);
    if (facing == Facing::East || facing == Facing::West)
    {
        movement.timeRequired = static_cast<int>(movement.timeRequired * horizontalMoveTimeMultiplier);
    }

    // Update tile passability
    const bool doesRouteContinue = !route.isEmpty();
    passabilityUpdater.onUnitLeavingTile(map, entity->getPos());
    passabilityUpdater.onUnitEnteringTile(map, movement.destination, doesRouteContinue);

    CollectionUtils::forEachWeakPtr<MovementListener>(
            listeners, [&](auto listener) { listener->onUnitMoveStart(&movement.destination); });
}

bool MovementComponent::tryRepathAroundNextNode(const PathfindingMap& map)
{
    const MapNode* nextNode = route.peek();
    if (passabilityChecker.isNodeObstructed(map, *nextNode))
    {
        // Tile is obstructed, e.g. another unit has stopped there
        if (!tryRepathAroundObstruction(map))
        {
            // No way around the obstruction - give up
            stopMovement();
            return false;
        }
    }
    else
    {
        // Tile is only temporarily obstructed, e.g. another unit is leaving the tile
        if (!tryRepathAroundTemporaryObstruction(map))
        {
            // We are either waiting to repath, or repathing failed
            return false;
        }
    }

    // We have found a route, but it might not currently be traversable
    nextNode = route.peek();
    if (!passabilityChecker.isNodeTraversable(map, *nextNode))
    {
        if (route.getSize() == 1)
        {
            // The pathfinder still returns a route of size 1 even if the destination is unreachable, which is
            // probably what's happened here. In this case there is no point trying to repath because we're already
            // about as close as we can get to destination.
            stopMovement();
        }
        else
        {
            // Still can't move yet!
            ++numFailedRepathAttempts;
            if (numFailedRepathAttempts >= maxRepathAttempts)
            {
                LOG_WARN_CATEGORY("movement", "Reached max repath attempts");
                stopMovement();
            }
        }

        return false;
    }

    return true;
}

bool MovementComponent::tryRepathAroundTemporaryObstruction(const PathfindingMap& map)
{
    if (ticksSpentWaiting < numTicksToWaitForTileToClear)
    {
        // Wait a while to see if the obstruction clears
        if (ticksSpentWaiting == 0)
        {
            CollectionUtils::forEachWeakPtr<MovementListener>(
                    listeners, [&](auto listener) { listener->onUnitPaused(); });
        }
        ++ticksSpentWaiting;
        return false;
    }

    ticksSpentWaiting = 0;

    return tryRepathAroundObstruction(map);
}

bool MovementComponent::tryRepathAroundObstruction(const PathfindingMap& map)
{
    /*
     * Repath, but treat the next node as an obstruction.
     *
     * This behavior is useful in several cases:
     * 1) Units can pathfind around bottlenecks, e.g. when moving a group around a corner, units would otherwise try to
     *    move in single-file and form a queue along the "optimal" route.
     * 2) Units can overtake slower units.
     * 3) Units can move out of each other's way if they are trying to walk through each other.
     *
     * Note that each time we attempt to repath around an obstruction, nodesToAvoid will grow.
     *
     * TODO: We should periodically check that these nodes are still obstructed, otherwise we may end up taking a
     * sub-optimal route.
     */
    const MapNode* nextNode = route.peek();
    cachedHints.nodesToAvoid.insert(*nextNode);

    if (!tryRepath(cachedHints))
    {
        // This should never happen, but just in case.
        // At the very least we would expect a path to the obstruction to be returned.
        return false;
    }

    nextNode = route.peek();

    if (!passabilityChecker.isNodeObstructed(map, *nextNode))
    {
        // We have found a way around the obstruction
        return true;
    }

    // Alternate route is still obstructed
    return false;
}

bool MovementComponent::tryRepath(Pathfinding::Hints hints)
{
    if (route.isEmpty())
    {
        return false;
    }

    // Prefer nodes from our current route; this saves us finding the entire path again
    auto existingPath = route.getPath();
    hints.nodesToPrefer.insert(existingPath.cbegin(), existingPath.cend());

    Pathfinding::Context context;
    // Don't try to repath to a destination that we know is unreachable
    const MapNode destination =
            route.isIntendedDestinationUnreachable() ? route.getFinalDestination() : route.getIntendedDestination();
    const bool success = moveTo(destination, context, hints);

    return success;
}

bool MovementComponent::tryRouteContinuation()
{
    const MapNode intendedDestination = route.getIntendedDestination();
    if (!intendedDestination.isValid())
    {
        // Route has been cleared, so we no longer have a valid destination
        return false;
    }

    if (route.getFinalDestination() == intendedDestination)
    {
        // Nothing to continue - we've reached our destination
        return false;
    }

    LOG_INFO_CATEGORY("movement", "Reached end of route but did not reach the intended destination!");

    // We have not yet reached our intended destination - try moving the rest of the way
    Pathfinding::Context context;
    Pathfinding::Hints hints;
    // Most of the hints are no longer relevant but we still care about the obstructedMovementCost
    hints.obstructedMovementCost = cachedHints.obstructedMovementCost;
    const bool success = moveTo(intendedDestination, context, cachedHints);

    if (success && route.getSize() < minSizeForRouteContinuation)
    {
        // Abort if the new route is sufficiently short.
        // This suggests that either:
        // 1) We are already very close to the intended destination.
        // 2) The intended destination is unreachable and we are as close as we can be.
        LOG_INFO_CATEGORY("movement", "Aborting route continuation");
        return false;
    }

    return success;
}

void MovementComponent::onLeftPreviousTile()
{
    // Update tile passability
    World* world = entity->getWorld();
    passabilityUpdater.onUnitLeftTile(*world, entity->getPos());

    // Update entity position
    entity->setPos(movement.destination);
}

void MovementComponent::onCompletedMoveToNewTile()
{
    movement.clear();

    if (!route.isEmpty())
    {
        // We still have more movement planned
        return;
    }

    if (tryRouteContinuation())
    {
        // We still have further to go
        return;
    }

    // Reached end of route
    stopMovement();
}

void MovementComponent::stopMovement()
{
    LOG_DEBUG_CATEGORY("movement", "Stopping movement");
    resetPassability(false);
    route = {};
    movement.clear();
    numFailedRepathAttempts = 0;
    ticksSpentWaiting = 0;
    CollectionUtils::forEachWeakPtr<MovementListener>(listeners, [&](auto listener) { listener->onUnitStopped(); });
}

}  // namespace Rival
