#pragma once

#include <deque>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>  // std::pair

#include "game/MapUtils.h"
#include "utils/HashUtils.h"

namespace std {

/** Custom hash function for std::pair<MapNode, MapNode>.
 * This must be defined *before* any use of std::unordered_map<std::pair<MapNode, MapNode>, ...>. */
template <>
struct hash<std::pair<Rival::MapNode, Rival::MapNode>>
{
    inline size_t operator()(const std::pair<Rival::MapNode, Rival::MapNode>& p) const noexcept
    {
        size_t seed = 0;
        Rival::HashUtils::hashCombineObj(seed, p.first);
        Rival::HashUtils::hashCombineObj(seed, p.second);
        return seed;
    }
};

}  // namespace std
namespace Rival {

class PathfindingMap;

namespace Pathfinding {

/**
 * Interface used to determine if a MapNode is traversable.
 */
class PassabilityChecker
{
public:
    /** Checks if a node can be included as part of a route. */
    virtual bool isNodePathable(const PathfindingMap& map, const MapNode& node) const = 0;

    /** Checks if a node is currently obstructed.
     * Obstructed tiles will only be included in a route as a last resort.
     * This serves 2 purposes:
     * 1) The obstruction may clear by the time we reach it.
     * 2) If the obstruction is an enemy unit, we can attack it upon reaching it. */
    virtual bool isNodeObstructed(const PathfindingMap& map, const MapNode& node) const = 0;

    /** Checks if a node can currently be moved into. */
    virtual bool isNodeTraversable(const PathfindingMap& map, const MapNode& node) const = 0;
};

/**
 * Interface used to modify passability in response to movement.
 */
class PassabilityUpdater
{
public:
    /** Called when a unit is due to try and move out of a tile. */
    virtual void onUnitPreparingMove(PathfindingMap& map, const MapNode& node) = 0;

    /** Called when a unit starts moving out of a tile. */
    virtual void onUnitLeavingTile(PathfindingMap& map, const MapNode& node) = 0;

    /** Called when a unit starts moving into a tile. */
    virtual void onUnitEnteringTile(PathfindingMap& map, const MapNode& node, bool isPassingThrough) = 0;

    /** Called when a unit has fully left a tile. */
    virtual void onUnitLeftTile(PathfindingMap& map, const MapNode& node) = 0;

    /** Called when a unit stops moving. */
    virtual void onUnitStopped(PathfindingMap& map, const MapNode& node) = 0;
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
 * Context object used to share information between separate pathfinding attempts.
 *
 * This allows us to save a lot of work during group pathfinding, when large portions of the path are likely to be
 * shared by multiple group members.
 */
class Context
{
public:
    Context(bool isCacheEnabled = false);

    /** Informs the context of a new pathfinding attempt. */
    void beginPathfinding();

    /** Gets the cached path from start to goal, if it exists. */
    std::optional<std::deque<MapNode>> getCachedPath(const MapNode& start, const MapNode& goal) const;

    /** Caches a path from start to goal. */
    void cachePath(const MapNode& start, const MapNode& goal, const std::deque<MapNode>& path);

private:
    std::unordered_map<std::pair<MapNode, MapNode>, std::deque<MapNode>> cachedPaths;

    bool isCacheEnabled = false;
    int pathfindingAttempts = 0;
};

/**
 * Hints that can be supplied during pathfinding to affect the route found.
 */
struct Hints
{
    /** Nodes that should be treated as obstructed. */
    std::unordered_set<MapNode> nodesToAvoid;
};

/**
 * Attempts to find the optimal path connecting `start` to `goal`.
 *
 * The start node is not included in the path.
 */
Route findPath(MapNode start,
        MapNode goal,
        const PathfindingMap& map,
        const PassabilityChecker& passabilityChecker,
        Context& context,
        const Hints hints = {});

}  // namespace Pathfinding
}  // namespace Rival
