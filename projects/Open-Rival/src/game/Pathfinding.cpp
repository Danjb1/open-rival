#include "game/Pathfinding.h"

#include <algorithm>   // std::min, std::none_of
#include <functional>  // std::greater
#include <iterator>    // std::back_inserter, std::next
#include <queue>
#include <unordered_map>
#include <vector>

#include "game/World.h"
#include "utils/LogUtils.h"

namespace Rival { namespace Pathfinding {

/**
 * A MapNode in the open list.
 * These are sortable such that the most promising nodes come first.
 */
struct OpenNode
{
    MapNode node;

    /** g(n): The measured cost from the start of the path to this node. */
    float costFromStart;

    /**
     * f(n): Our current best guess as to how short a path from start to finish can be if it goes through this node.
     *
     * This is calculated as:
     *
     *     g(n) + h(n)
     *
     * where g(n) is the cost from the start to this node, and h is our heuristic function (estimates the cost from a
     * node to the goal).
     */
    float totalPathCost;

    /** Whether this node should be preferred over others of equal cost (see Hints::nodesToPrefer). */
    bool isPreferred = false;

    /**
     * Determines whether this node is less promising than another.
     *
     * Nodes with the lowest f(n) come first. Ties are common, since many routes have the same cost, so we break them
     * (in order of priority) by:
     *
     *   1) Preferring nodes that we have been asked to prefer, e.g. nodes from a previous route.
     *   2) Preferring nodes with the highest g(n), i.e. the furthest along their route. This makes the pathfinder
     *      commit to one of several equally-good routes, rather than exploring them all.
     *
     * Exact comparisons are fine here: movement costs are typically multiples of 0.5, which floats represent exactly.
     * Even if they weren't, we would merely detect fewer ties; this only affects efficiency, not correctness.
     */
    bool operator>(const OpenNode& other) const
    {
        if (totalPathCost != other.totalPathCost)
        {
            return totalPathCost > other.totalPathCost;
        }
        if (isPreferred != other.isPreferred)
        {
            return other.isPreferred;
        }
        return costFromStart < other.costFromStart;
    }
};

/** A path found by the Pathfinder, along with its movement cost. */
struct FoundPath
{
    std::deque<MapNode> path;
    float cost = 0.f;
};

/**
 * Describes a MapNode that is part of a path.
 */
struct NodePathInfo
{
    MapNode prev;
    float costFromStart;
};

/**
 * Temporary object used in pathfinding.
 *
 * For now this uses a simple A* search. Later, we could investigate the use
 * of waypoints if this does not perform adequately.
 */
class Pathfinder
{
public:
    Pathfinder(MapNode start,
            MapNode goal,
            const PathfindingMap& map,
            const PassabilityChecker& passabilityChecker,
            Context& context,
            const Hints& hints);

    Route run();

private:
    FoundPath findPath();
    bool isFinished() const;
    float estimateCostToGoal(const MapNode& node) const;
    static float estimateOpenGroundCost(const MapNode& from, const MapNode& to);
    FoundPath reconstructPath(const MapNode& destination) const;
    FoundPath reconstructPathWithCache(const MapNode& cachedRouteStart, const CachedRoute& cachedRoute) const;
    void cacheRoute(const std::deque<MapNode>& path) const;
    std::vector<MapNode> findPathableNeighbors(const MapNode& node) const;
    float getCostToNode(const MapNode& node) const;
    float getMovementCost(const MapNode& from, const MapNode& to) const;
    bool isNodePreferred(const MapNode& node) const;

private:
    /** Base movement cost for moving between two tiles. */
    static constexpr float baseMovementCost = 1.f;

    /*
     * Movement cost multiplier for horizontal movement.
     *
     * This warrants some explanation.
     *
     * Imagine you want to move 2 tiles north-east (A -> C):
     *
     *           ,x
     *         ,x C`x
     *       ,x B`x'
     *      x A`x'D`x
     *       `x' `x'
     *
     * This can be accomplished 2 different ways:
     *   A -> B -> C
     *   A -> D -> C
     *
     * Both routes involve 2 movements, but it would look strange if we
     * chose the second route because the diagonal route appears more
     * logical and direct.
     *
     * To ensure that the first route gets chosen, we consider east and west
     * movements to be slightly more expensive than other movements.
     * Crucially, they are still cheaper than 2 diagonals movements, so
     * units will still move directly east/west when it makes sense to do
     * so.
     */
    static constexpr float horizontalMoveCostMultiplier = 1.5f;

    /** In the case of an unreachable goal, this is the maximum number of nodes to expand until we abort, if we don't
     * find a closer one. Increasing this may allow units to find a closer point to the goal, at the cost of additional
     * work. */
    static constexpr int closestPointSearchLimit = 50;

    /** The starting node. */
    MapNode start;

    /** The destination node. */
    MapNode goal;

    /** All nodes that should be considered valid destinations. */
    std::unordered_set<MapNode> goalNodes;

    /** The PathfindingMap used to find obstacles, etc. */
    const PathfindingMap& map;

    /** Object used to check for passability. */
    const PassabilityChecker& passabilityChecker;

    /** Active pathfinding context. */
    Context& context;

    /** Hints used during pathfinding. */
    const Hints& hints;

    /** The "open list", i.e. a queue of nodes to consider. */
    std::priority_queue<OpenNode, std::vector<OpenNode>, std::greater<>> open;

    /** Map of MapNode -> NodePathInfo, for all nodes visited (the "closed list"). */
    std::unordered_map<MapNode, NodePathInfo> nodeInfo;

    /** The lowest cost found to the goal. */
    float lowestCostToGoal = std::numeric_limits<float>::max();

    /** The closest node found to the goal. */
    MapNode closestNodeToGoal = MapNode::Invalid;

    /** Number of nodes visited during pathfinding. */
    int nodesVisited = 0;

    /** In the case of an unreachable goal, this is the number of nodes visited without finding a closer one. */
    int nodesVisitedSinceImprovement = 0;

    /** Whether the goal is unreachable from the start tile. */
    bool isGoalUnreachable = false;

    /** Whether the pathfinder aborted early due to the goal being too far away. */
    bool isPartialPath = false;
};

/**
 * Constructs a Pathfinder which attempts to find a path connecting start to goal.
 */
Pathfinder::Pathfinder(MapNode start,
        MapNode goal,
        const PathfindingMap& map,
        const PassabilityChecker& passabilityChecker,
        Context& context,
        const Hints& hints)
    : start(start)
    , goal(goal)
    , map(map)
    , passabilityChecker(passabilityChecker)
    , context(context)
    , hints(hints)
{
}

Route Pathfinder::run()
{
    FoundPath found = findPath();
    return { start, goal, std::move(found.path), found.cost, isGoalUnreachable, isPartialPath };
}

/**
 * Attempts to find a path based on the Pathfinder's configuration.
 */
FoundPath Pathfinder::findPath()
{
    context.beginPathfinding();

    if (start == goal)
    {
        return {};
    }

    LOG_DEBUG_CATEGORY("pathfinding", "Planning route to ({}, {})", goal.x, goal.y);

    closestNodeToGoal = start;
    nodeInfo[start] = { MapNode::Invalid, 0.f };

    // If the goal tile is not pathable, we should also consider its neighbors as valid goal tiles.
    // This prevents us giving up on an "unreachable" goal if a tree or a building is clicked.
    goalNodes = { goal };
    if (!passabilityChecker.isNodePathable(map, goal))
    {
        std::vector<MapNode> neighbors = findPathableNeighbors(goal);
        goalNodes.insert(neighbors.cbegin(), neighbors.cend());
    }

    // The goal is only unreachable if none of the acceptable goal nodes can be reached
    isGoalUnreachable = std::none_of(goalNodes.cbegin(), goalNodes.cend(), [&](const MapNode& goalNode) {
        return map.isReachable(start, goalNode);
    });

    open.push({ start, 0.f, estimateCostToGoal(start) });

    const int maxNodesToVisit = context.getMaxNodesToVisit();

    while (!isFinished())
    {
        // Abort if we've exceeded our pathfinding allowance
        if (nodesVisited >= maxNodesToVisit)
        {
            LOG_WARN_CATEGORY("pathfinding", "Exceeded maximum nodes to visit ({})", maxNodesToVisit);
            isPartialPath = true;
            break;
        }

        // Pop the next best candidate for our path
        const OpenNode current = open.top();
        open.pop();

        if (current.costFromStart > getCostToNode(current.node))
        {
            // A cheaper route to this node was found after this entry was queued
            continue;
        }

        // Abort if we've reached the goal
        if (goalNodes.contains(current.node))
        {
            LOG_INFO_CATEGORY("pathfinding", "Found goal in {} steps", nodesVisited);
            FoundPath found = reconstructPath(current.node);
            cacheRoute(found.path);
            return found;
        }

        // Abort if we've reached a tile which has a cached route to the goal
        if (const auto cachedRoute = context.getCachedRoute(current.node, goal))
        {
            LOG_INFO_CATEGORY("pathfinding",
                    "Found goal in {} steps using cached route from ({}, {})",
                    nodesVisited,
                    current.node.x,
                    current.node.y);
            return reconstructPathWithCache(current.node, *cachedRoute);
        }

        // Abort if we know the goal is unreachable and we haven't found a closer point recently.
        // This means we've probably found the closest point we can reach.
        if (isGoalUnreachable)
        {
            ++nodesVisitedSinceImprovement;
            if (nodesVisitedSinceImprovement > closestPointSearchLimit)
            {
                LOG_INFO_CATEGORY("pathfinding", "Aborting pathfinding for unreachable destination");
                break;
            }
        }

        // Abort if our path has become too expensive
        if (current.costFromStart > hints.maxPathCost)
        {
            LOG_WARN_CATEGORY(
                    "pathfinding", "Exceeded maximum path cost: {} / {}", current.costFromStart, hints.maxPathCost);
            isPartialPath = true;
            break;
        }

        // Evaluate neighbours
        const float currentCost = current.costFromStart;
        std::vector<MapNode> neighbors = findPathableNeighbors(current.node);

        for (const MapNode& neighbor : neighbors)
        {
            const float newCostToNeighbor = currentCost + getMovementCost(current.node, neighbor);
            const float bestCostToNeighbor = getCostToNode(neighbor);

            if (newCostToNeighbor >= bestCostToNeighbor)
            {
                // We already have a better path to this neighbor
                continue;
            }

            // This path to neighbor is better than any previous one
            const float estimatedCostToGoal = estimateCostToGoal(neighbor);
            nodeInfo[neighbor] = { current.node, newCostToNeighbor };
            open.push({ neighbor,
                    newCostToNeighbor,
                    newCostToNeighbor + estimatedCostToGoal,
                    isNodePreferred(neighbor) });

            if (estimatedCostToGoal < lowestCostToGoal)
            {
                // We have found a new closest point to the goal
                lowestCostToGoal = estimatedCostToGoal;
                closestNodeToGoal = neighbor;
                nodesVisitedSinceImprovement = 0;
            }
        }

        ++nodesVisited;
    }

    // The goal could not be reached - get as close as we can.
    // Note that, due to the fact that due to the fact that we do not set lowestCostToGoal for the start node,
    // we will always move at least 1 tile (if possible), even if it takes us further away than our starting
    // location. This is in keeping with the behavior of the original game.
    LOG_INFO_CATEGORY("pathfinding", "Found best path towards goal in {} steps", nodesVisited);
    return reconstructPath(closestNodeToGoal);
}

bool Pathfinder::isFinished() const
{
    return open.empty();
}

/**
 * Heuristic function used to estimate the cost from a MapNode to the goal.
 */
float Pathfinder::estimateCostToGoal(const MapNode& node) const
{
    float best = std::numeric_limits<float>::max();
    for (const MapNode& goalNode : goalNodes)
    {
        best = std::min(best, estimateOpenGroundCost(node, goalNode));
    }
    return best;
}

/**
 * Calculates the cost of the cheapest route between 2 nodes, assuming there are no obstacles.
 *
 * This is used as our heuristic. Since obstacles only ever make a route more expensive, this never overestimates the
 * true cost, which means the pathfinder finds the cheapest route. And since it is exact in open ground, the pathfinder
 * doesn't waste time exploring unnecessary nodes.
 *
 * The cheapest route uses as many diagonal movements as possible, then covers the remaining distance with east/west or
 * north/south movements (see MapUtils::TileOffset for how far each movement goes).
 */
float Pathfinder::estimateOpenGroundCost(const MapNode& from, const MapNode& to)
{
    // This calculation assumes that diagonal movements are preferable to a combination of horizontal and vertical
    // movements, and that horizontal movements are preferable to 2 diagonal movements
    static_assert(horizontalMoveCostMultiplier > 1.f && horizontalMoveCostMultiplier <= 2.f);

    const MapUtils::TileOffset offset = MapUtils::getTileOffset(from, to);
    const int diagonalMoves = std::min(offset.columns, offset.halfRows);

    // After moving diagonally, the remaining distance is either 0, or the difference between `columns` and `halfRows`.
    // This difference is always even (see MapUtils::TileOffset), so these divisions are exact.
    const int horizontalMoves = (offset.columns - diagonalMoves) / 2;
    const int verticalMoves = (offset.halfRows - diagonalMoves) / 2;

    return baseMovementCost * (diagonalMoves + verticalMoves + horizontalMoves * horizontalMoveCostMultiplier);
}

/**
 * Returns the path found from the start to the given MapNode.
 */
FoundPath Pathfinder::reconstructPath(const MapNode& destination) const
{
    FoundPath found;
    found.cost = getCostToNode(destination);

    // Follow the previous nodes back to the start
    MapNode currentNode = destination;
    while (currentNode != start)
    {
        found.path.push_front(currentNode);

        const auto it = nodeInfo.find(currentNode);
        if (it == nodeInfo.cend())
        {
            // No previous node found. This should never happen since we don't enter the loop for the start node.
            break;
        }
        currentNode = it->second.prev;
    }

    return found;
}

/**
 * Returns the path from the start to `cachedRouteStart`, followed by the rest of the route from the cache.
 */
FoundPath Pathfinder::reconstructPathWithCache(const MapNode& cachedRouteStart, const CachedRoute& cachedRoute) const
{
    FoundPath found = reconstructPath(cachedRouteStart);
    found.path.insert(found.path.end(), cachedRoute.path.cbegin(), cachedRoute.path.cend());
    found.cost += cachedRoute.cost;
    return found;
}

/**
 * Caches each step of a complete path, so that later searches in the same Context can join it part-way along.
 */
void Pathfinder::cacheRoute(const std::deque<MapNode>& path) const
{
    if (path.empty())
    {
        return;
    }

    const float totalCost = getCostToNode(path.back());
    MapNode from = start;
    for (const MapNode& next : path)
    {
        context.cacheStep(from, goal, next, totalCost - getCostToNode(from));
        from = next;
    }
}

/**
 * Returns a vector containing all valid (pathable) neighbors of the given MapNode.
 */
std::vector<MapNode> Pathfinder::findPathableNeighbors(const MapNode& node) const
{
    std::vector<MapNode> allNeighbors = MapUtils::findNeighbors(node, map);

    // Filter out non-pathable neighbors
    std::vector<MapNode> validNeighbors;
    validNeighbors.reserve(MapUtils::maxNeighbors);
    std::copy_if(allNeighbors.begin(), allNeighbors.end(), std::back_inserter(validNeighbors), [this](MapNode n) {
        return this->passabilityChecker.isNodePathable(this->map, n);
    });

    return validNeighbors;
}

/**
 * Gets the cost of moving from the start to the given MapNode.
 *
 * Returns the integer max if no path has been found yet.
 */
float Pathfinder::getCostToNode(const MapNode& node) const
{
    const auto it = nodeInfo.find(node);
    if (it == nodeInfo.cend())
    {
        // No path to node found yet
        return std::numeric_limits<float>::max();
    }
    return it->second.costFromStart;
}

/**
 * Gets the cost of moving to a neighboring tile.
 */
float Pathfinder::getMovementCost(const MapNode& from, const MapNode& to) const
{
    const Facing movementDir = MapUtils::getDir(from, to);
    const float dirMultiplier =
            (movementDir == Facing::East || movementDir == Facing::West) ? horizontalMoveCostMultiplier : 1.f;

    if (goalNodes.contains(to))
    {
        // We don't care if the goal is obstructed since there's no way around it,
        // so just return the basic movement cost.
        return baseMovementCost * dirMultiplier;
    }

    // Cost is increased for obstructed nodes or nodes to avoid
    const bool shouldAvoidNode = hints.nodesToAvoid.contains(to);
    const bool isObstructed = passabilityChecker.isNodeObstructed(map, to);
    const float baseCost = (shouldAvoidNode || isObstructed) ? hints.obstructedMovementCost : baseMovementCost;
    return baseCost * dirMultiplier;
}

/**
 * Determines whether a node should be preferred over others of equal cost.
 *
 * Note that preferred nodes are not cheaper to move to; they are just explored first (see OpenNode).
 */
bool Pathfinder::isNodePreferred(const MapNode& node) const
{
    return !hints.nodesToAvoid.contains(node) && hints.nodesToPrefer.contains(node);
}

Route::Route(MapNode start,
        MapNode destination,
        std::deque<MapNode> path,
        float cost,
        bool bIsIntendedDestinationUnreachable,
        bool bIsPartialPath)
    : intendedDestination(destination)
    , path(path)
    , cost(cost)
    , bIsIntendedDestinationUnreachable(bIsIntendedDestinationUnreachable)
    , bIsPartialPath(bIsPartialPath)
{
    foundDestination = path.empty() ? start : path.back();
}

bool Route::isEmpty() const
{
    return path.size() == 0;
}

MapNode Route::pop()
{
    const MapNode node = path.front();
    path.pop_front();
    return node;
}

const MapNode* Route::peek() const
{
    return path.empty() ? nullptr : &path.front();
}

Route findPath(MapNode start,
        MapNode goal,
        const PathfindingMap& map,
        const PassabilityChecker& passabilityChecker,
        Context& context,
        const Hints& hints)
{
    Pathfinder pathfinder = Pathfinder(start, goal, map, passabilityChecker, context, hints);
    return pathfinder.run();
}

Context::Context(int numUnits)
    : numUnits(numUnits)
    , maxNodesToVisitPerAttempt(std::max(static_cast<int>(maxNodesToVisitPerContext / numUnits), maxNodesLowerLimit))
    , isCacheEnabled(numUnits > 1)
{
}

void Context::beginPathfinding()
{
    ++pathfindingAttempts;
}

std::optional<CachedRoute> Context::getCachedRoute(const MapNode& from, const MapNode& to) const
{
    const CachedStep* step = findCachedStep(from, to);
    if (!step)
    {
        // No cached path exists
        return {};
    }

    CachedRoute route;
    route.cost = step->remainingCost;

    // Each CachedStep records the next step in the path {from, to}.
    // Follow the chain of steps until we reach a node with no cached step.
    while (step)
    {
        route.path.push_back(step->next);
        step = findCachedStep(step->next, to);
    }

    return route;
}

/** Returns the cached step from `from` towards `to`, or nullptr if there isn't one. */
const Context::CachedStep* Context::findCachedStep(const MapNode& from, const MapNode& to) const
{
    const auto iter = cachedSteps.find({ from, to });
    return iter != cachedSteps.cend() ? &iter->second : nullptr;
}

void Context::cacheStep(const MapNode& from, const MapNode& to, const MapNode& next, float remainingCost)
{
    if (!isCacheEnabled)
    {
        return;
    }

    // NOTE: The first route cached for a node wins.
    // Subsequent searches will use the cached route.
    cachedSteps.try_emplace(std::make_pair(from, to), CachedStep { next, remainingCost });
}

int Context::getMaxNodesToVisit() const
{
    if (pathfindingAttempts == 1)
    {
        // Don't restrict the first attempt, as the results of this can assist with subsequent attempts
        return maxNodesToVisitPerContext;
    }

    return maxNodesToVisitPerAttempt;
}

}}  // namespace Rival::Pathfinding
