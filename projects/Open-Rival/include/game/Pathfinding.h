#pragma once

#include <deque>

#include "game/MapUtils.h"

namespace Rival {

class PathfindingMap;
class WritablePathfindingMap;

namespace Pathfinding {

/**
 * Interface used to determine if a MapNode is traversable.
 */
class PassabilityChecker
{
public:
    virtual bool isNodePathable(const PathfindingMap& map, const MapNode& node) const = 0;
    virtual bool isNodeTraversable(const PathfindingMap& map, const MapNode& node) const = 0;
};

/**
 * Interface used to modify passability in response to movement.
 */
class PassabilityUpdater
{
public:
    virtual void onUnitLeavingTile(WritablePathfindingMap& map, const MapNode& node) = 0;
    virtual void onUnitEnteringTile(WritablePathfindingMap& map, const MapNode& node) = 0;
    virtual void onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node) = 0;
    virtual void onUnitEnteredTile(WritablePathfindingMap& map, const MapNode& node) = 0;
};

/**
 * A planned path to a destination.
 */
class Route
{
public:
    /**
     * Default constructor; makes a Route with an empty path.
     */
    Route();

    /**
     * Constructs a Route with a path and destination.
     */
    Route(MapNode destination, std::deque<MapNode> path);

    /**
     * Determines if this Route is empty.
     */
    bool isEmpty() const;

    MapNode getDestination() const
    {
        return destination;
    }

    /**
     * Removes the next MapNode from the path and returns it.
     */
    MapNode pop();

    /**
     * Returns a pointer to the next MapNode from the path, without removing
     * it.
     *
     * Returns nullptr if the path is empty.
     */
    const MapNode* peek() const;

private:
    MapNode destination;
    std::deque<MapNode> path;
};

/**
 * Attempts to find the optimal path connecting `start` to `goal`.
 *
 * The start node is not included in the path.
 */
Route findPath(MapNode start, MapNode goal, const PathfindingMap& map, const PassabilityChecker& passabilityChecker);

}  // namespace Pathfinding
}  // namespace Rival
