#pragma once

#include <deque>
#include <limits>  // numeric_limits
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

    /** Called when a units aborts a movement into a tile. */
    virtual void onUnitMoveAborted(PathfindingMap& map, const MapNode& node) = 0;
};

/**
 * A planned path to a destination.
 */
class Route
{
public:
    /** Default constructor; makes a Route with an empty path. */
    Route();

    /** Constructs a Route with a path and destination. */
    Route(MapNode start,
            MapNode destination,
            std::deque<MapNode> path,
            float cost,
            bool isIntendedDestinationUnreachable);

    /** Determines if this Route is empty. */
    bool isEmpty() const;

    /** Gets the destination that was originally intended when planning this route. */
    MapNode getIntendedDestination() const
    {
        return intendedDestination;
    }

    /** Gets the destination that was ultimately found when planning this route. */
    MapNode getFinalDestination() const
    {
        return foundDestination;
    }

    /** Removes the next MapNode from the path and returns it. */
    MapNode pop();

    /** Returns a pointer to the next MapNode from the path, without removing it.
     * Returns nullptr if the path is empty. */
    const MapNode* peek() const;

    /** Gets the number of tiles traversed by this route. */
    int getSize() const
    {
        return static_cast<int>(path.size());
    }

    /** Gets the movement cost of this route. */
    float getMovementCost() const
    {
        return cost;
    }

    /** Gets the full path for this route. */
    std::deque<MapNode> getPath() const
    {
        return path;
    }

    /** Returns true if the intended destination was *definitely* unreachable.
     * Note that even if this returns false, the destiantion might still be unreachable. */
    bool isIntendedDestinationUnreachable()
    {
        return bIsIntendedDestinationUnreachable;
    }

private:
    MapNode intendedDestination;
    MapNode foundDestination;
    std::deque<MapNode> path;
    float cost = 0;
    bool bIsIntendedDestinationUnreachable = false;
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
    Context(int numUnits = 1);

    /** Informs the context of a new pathfinding attempt. */
    void beginPathfinding();

    /** Gets the cached path from start to goal, if it exists. */
    std::optional<std::deque<MapNode>> getCachedPath(const MapNode& start, const MapNode& goal) const;

    /** Gets the cost of the cached path from start to goal, if it exists. */
    std::optional<float> getCachedPathCost(const MapNode& start, const MapNode& goal) const;

    /** Caches a path from start to goal. */
    void cachePath(const MapNode& start, const MapNode& goal, const std::deque<MapNode>& path, float pathCost);

    /** Registers a goal node to be considered for subsequent pathfinding attempts. */
    void registerGoalNode(MapNode goal);

    /** Adds all registered goal nodes to the given set. */
    void addRegisteredGoalNodes(std::unordered_set<MapNode>& goalNodes);

    /** Gets the maximum nodes of nodes that the pathfinder should visit on the current attempt. */
    int getMaxNodesToVisit() const;

private:
    /** Maximum nodes that the pathfinder can visit before giving up.
     * This is set low enough that it should not cause any lag spikes.
     * In general we should not reach this limit unless pathfinding across vast distances;
     * pathfinding in "normal" conditions typically requires 1000 or fewer nodes. */
    static constexpr int maxNodesToVisitPerContext = 2000;

    /** Lower limit for maxNodesToVisitPerAttempt.
     * This prevents pathfinding from becoming very bad when moving large groups. */
    static constexpr int maxNodesLowerLimit = 100;

    std::unordered_set<MapNode> registeredGoalNodes;

    std::unordered_map<std::pair<MapNode, MapNode>, std::deque<MapNode>> cachedPaths;
    std::unordered_map<std::pair<MapNode, MapNode>, float> cachedPathCosts;

    int pathfindingAttempts = 0;

    int numUnits = 0;

    int maxNodesToVisitPerAttempt = 0;

    bool isCacheEnabled = false;
};

/**
 * Hints that can be supplied during pathfinding to affect the route found.
 */
struct Hints
{
    /** Nodes that should be treated as obstructed. */
    std::unordered_set<MapNode> nodesToAvoid;

    /** Nodes that should be preferred (if they are not also present in nodesToAvoid). */
    std::unordered_set<MapNode> nodesToPrefer;

    /**
     * Movement cost when trying to move into a tile that is currently obstructed.
     *
     * This should vary based on context. For example, units that are trying to attack should be more determined to
     * reach the target.
     *
     * Note that when moving a group, obstructions at the destination are inevitable.
     *
     * This is tricky to get right:
     * - If this is too low, units will not be able to pathfind around obstructions in cases where the alternative route
     *   is long.
     * - If this is too high, group movement becomes computationally expensive because units will waste their time
     *   trying long alternative routes to avoid (inevitable) obstructions.
     *
     * TODO: To help with this, we could make obstructions more expensive the further they are from the goal. This would
     * mean that units try harder to pathfind around obstructions when they still have a long way to travel, but they
     * become "lazier" as they near the destination.
     */
    float obstructedMovementCost = 3.f;

    /** Maximum path cost before pathfinding will abort. */
    float maxPathCost = std::numeric_limits<float>::max();
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
