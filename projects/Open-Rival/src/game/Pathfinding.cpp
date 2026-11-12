#include "game/Pathfinding.h"

#include <algorithm>  // min, reverse
#include <iterator>   // back_inserter, next
#include <queue>
#include <unordered_map>
#include <vector>

#include "game/World.h"
#include "utils/LogUtils.h"

namespace Rival { namespace Pathfinding {

/**
 * A MapNode with an associated score for pathfinding.
 */
struct ReachableNode
{
    MapNode node;

    /**
     * Our current best guess as to how short a path from start to finish
     * can be if it goes through this node.
     *
     * This is calculated as:
     *
     *     fScore + h(n)
     *
     * where fScore is the cost from the start to this node, and h is our
     * heuristic function (estimates the cost from a node to the goal).
     */
    float cost;

    bool operator<(const ReachableNode& other) const
    {
        return cost < other.cost;
    }

    bool operator>(const ReachableNode& other) const
    {
        return other < *this;
    }
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
            const Hints hints);

    Route getRoute() const
    {
        return route;
    }

private:
    std::deque<MapNode> findPath();
    bool isFinished() const;
    ReachableNode popBestNode();
    float estimateCostToGoal(const MapNode& node);
    std::deque<MapNode> reconstructPath(const MapNode& node, bool shouldCachePaths = false) const;
    std::deque<MapNode> reconstructPathWithCache(const MapNode& node, const std::deque<MapNode>& cachedPath);
    std::vector<MapNode> findPathableNeighbors(const MapNode& node) const;
    float getCostToNode(const MapNode& node) const;
    float getMovementCost(const MapNode& from, const MapNode& to) const;
    void updatePathToNode(const MapNode& node, float newCost);
    ReachableNode* findDiscoveredNode(const MapNode& node);

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

    /** Discount applied to the movement cost of nodes that should be preferred. */
    static constexpr float preferredNodeDiscount = 0.1f;

    /** In the case of an unreachable goal, this is the maximum number of nodes to expand until we abort, if we don't
     * find a closer one. Increasing this may allow units to find a closer point to the goal, at the cost of additional
     * work. */
    static constexpr int closestPointSearchLimit = 30;

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
    Hints hints;

    /** All discovered nodes, sorted with the "best" nodes first (the "open list"). */
    std::vector<ReachableNode> discoveredNodes;

    /** Map of node -> lowest cost to reach that node from the start (the "closed list"). */
    std::unordered_map<MapNode, float> costToNode;

    /** Map of node -> estimated cost to reach the goal. */
    std::unordered_map<MapNode, float> cachedCostToGoal;

    /** Map of node -> previous node in the shortest path found. */
    std::unordered_map<MapNode, MapNode> prevNode;

    /** Unvisited nodes that are adjacent to at least one visited node. */
    std::unordered_set<MapNode> perimeter;

    /** After construction, contains the shortest route to the goal. */
    Route route;

    /** The lowest cost found to the goal. */
    float lowestCostToGoal = std::numeric_limits<float>::max();

    /** The closest node found to the goal. */
    MapNode closestNodeToGoal;

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
        const Hints hints)
    : start(start)
    , goal(goal)
    , map(map)
    , passabilityChecker(passabilityChecker)
    , context(context)
    , hints(hints)
{
    const auto path = findPath();

    float pathCost = 0;
    if (!path.empty())
    {
        const auto destination = path.back();
        pathCost = costToNode[destination];
    }

    route = { start, goal, path, pathCost, isGoalUnreachable, isPartialPath };
}

/**
 * Attempts to find a path based on the Pathfinder's configuration.
 */
std::deque<MapNode> Pathfinder::findPath()
{
    context.beginPathfinding();

    if (start == goal)
    {
        return {};
    }

    LOG_DEBUG_CATEGORY("pathfinding", "Planning route to ({}, {})", goal.x, goal.y);

    isGoalUnreachable = !map.isReachable(start, goal);
    discoveredNodes.push_back({ start, 0 });
    closestNodeToGoal = start;
    costToNode[start] = 0;

    // If the goal tile is not pathable, we should also consider its neighbors as valid goal tiles.
    // If none of *those* tiles are pathable, then that's ok; our flood fill algorithm will kick in later to detect
    // the goal as being unreachable.
    goalNodes = { goal };
    if (!passabilityChecker.isNodePathable(map, goal))
    {
        std::vector<MapNode> neighbors = findPathableNeighbors(goal);
        goalNodes.insert(neighbors.cbegin(), neighbors.cend());
    }

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

        const ReachableNode current = popBestNode();

        // Abort if we've reached the goal
        if (goalNodes.contains(current.node))
        {
            LOG_INFO_CATEGORY("pathfinding", "Found goal in {} steps", nodesVisited);
            return reconstructPath(current.node, /* cachePaths = */ true);
        }

        // Abort if we've reached a tile which has a cached path to the goal
        auto cachedPath = context.getCachedPath(current.node, goal);
        if (cachedPath.has_value())
        {
            // Use the cached path to the goal from this node onwards
            LOG_INFO_CATEGORY("pathfinding",
                    "Found goal in {} steps using cached path from ({}, {})",
                    nodesVisited,
                    current.node.x,
                    current.node.y);
            return reconstructPathWithCache(current.node, *cachedPath);
        }

        // Abort if we know the goal is unreachable, and the next node takes us significantly further away from the
        // goal. This means we've probably found the closest point we can reach.
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
        if (current.cost > hints.maxPathCost)
        {
            LOG_WARN_CATEGORY("pathfinding", "Exceeded maximum path cost: {} / {}", current.cost, hints.maxPathCost);
            isPartialPath = true;
            break;
        }

        std::vector<MapNode> neighbors = findPathableNeighbors(current.node);

        for (const MapNode& neighbor : neighbors)
        {
            const float bestCostToNeighbor = getCostToNode(neighbor);
            const float newCostToNeighbor = getCostToNode(current.node) + getMovementCost(current.node, neighbor);
            if (newCostToNeighbor < bestCostToNeighbor)
            {
                // This path to neighbor is better than any previous one
                costToNode[neighbor] = newCostToNeighbor;
                prevNode[neighbor] = current.node;
                updatePathToNode(neighbor, newCostToNeighbor);
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
    return discoveredNodes.empty();
}

/**
 * Removes the ReachableNode with the lowest estimated cost from the list
 * of discovered Nodes, and returns it.
 */
ReachableNode Pathfinder::popBestNode()
{
    // Making `discoveredNodes` into a min-heap means we can extract the
    // best node in O(1) time.
    std::make_heap(discoveredNodes.begin(), discoveredNodes.end(), std::greater<ReachableNode> {});
    // After calling `pop_heap`, the best node is at the back of the list
    std::pop_heap(discoveredNodes.begin(), discoveredNodes.end(), std::greater<ReachableNode> {});
    ReachableNode bestNode = discoveredNodes.back();
    discoveredNodes.pop_back();
    return bestNode;
}

/**
 * Heuristic function used to estimate the cost from a MapNode to the goal.
 */
float Pathfinder::estimateCostToGoal(const MapNode& node)
{
    if (goalNodes.contains(node))
    {
        return 0.f;
    }

    const auto iter = cachedCostToGoal.find(node);
    if (iter != cachedCostToGoal.cend())
    {
        return iter->second;
    }

    // Estimate the cost to each goal and pick the lowest
    float best = std::numeric_limits<float>::max();
    for (const auto& possibleGoal : goalNodes)
    {
        const int dx = abs(node.x - possibleGoal.x);
        const int dy = abs(node.y - possibleGoal.y);

        // Whatever distance x and y have in common can be covered diagonally
        const int diagonalDistance = std::min(dx, dy);
        const int remainingDistance = abs(dx - dy);

        const float costToGoal = static_cast<float>(diagonalDistance + remainingDistance);

        best = std::min(costToGoal, best);
    }

    cachedCostToGoal.emplace(node, best);

    return best;
}

/**
 * Returns the path found from the start to the given MapNode.
 */
std::deque<MapNode> Pathfinder::reconstructPath(const MapNode& destination, bool shouldCachePaths) const
{
    std::deque<MapNode> path = {};
    MapNode currentNode = destination;
    const float costToFinalNode = getCostToNode(destination);

    // Follow the previous nodes back to the start
    while (currentNode != start)
    {
        path.push_front(currentNode);

        if (shouldCachePaths && currentNode != destination)
        {
            // Cache every partial path that we find along the way.
            // Note that cached paths always include the start node, e.g. a cached path from (2, 2) to (4, 4) starts
            // with the node (2, 2).
            const float costToCurrentNode = getCostToNode(currentNode);
            const float partialPathCost = costToFinalNode - costToCurrentNode;
            context.cachePath(currentNode, destination, path, partialPathCost);
        }

        const auto it = prevNode.find(currentNode);
        if (it == prevNode.cend())
        {
            // No previous node found. This should never happen since we
            // don't enter the loop for the start node.
            break;
        }
        currentNode = it->second;
    }

    return path;
}

std::deque<MapNode> Pathfinder::reconstructPathWithCache(
        const MapNode& cachedPathStart, const std::deque<MapNode>& cachedPath)
{
    // Get the path from start -> node
    std::deque<MapNode> path = reconstructPath(cachedPathStart);

    // Append the path from next node -> goal.
    // This is the cached path minus the first element (which is node again).
    path.insert(path.end(), std::next(cachedPath.begin()), cachedPath.end());

    // Determine the final path cost
    const std::optional<float> cachedPathCost = context.getCachedPathCost(cachedPathStart, goal);
    const float totalCost = costToNode[cachedPathStart] + *cachedPathCost;
    costToNode[goal] = totalCost;

    return path;
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
    const auto it = costToNode.find(node);
    if (it == costToNode.cend())
    {
        // No path to node found yet
        return std::numeric_limits<float>::max();
    }
    return it->second;
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
    float finalCost = baseCost * dirMultiplier;

    // Preferred nodes get a slight discount
    const bool shouldPreferNode = !shouldAvoidNode && hints.nodesToPrefer.contains(to);
    if (shouldPreferNode)
    {
        finalCost -= preferredNodeDiscount;
    }

    return finalCost;
}

/**
 * Updates the path to a node with a shorter one, or adds a new path to the node if this is the first one found.
 *
 * Also records the closest node to the goal.
 */
void Pathfinder::updatePathToNode(const MapNode& node, float newCost)
{
    const float estimatedCostToGoal = estimateCostToGoal(node);
    const float newEstimate = newCost + estimatedCostToGoal;

    if (ReachableNode* previouslyDiscoveredNode = findDiscoveredNode(node))
    {
        previouslyDiscoveredNode->cost = newEstimate;
    }
    else
    {
        discoveredNodes.emplace_back(node, newEstimate);
    }

    if (estimatedCostToGoal < lowestCostToGoal)
    {
        // We have found a new closest point to the goal
        lowestCostToGoal = estimatedCostToGoal;
        closestNodeToGoal = node;
        nodesVisitedSinceImprovement = 0;
    }
}

/**
 * Finds the ReachableNode associated with the given MapNode, if present.
 */
ReachableNode* Pathfinder::findDiscoveredNode(const MapNode& node)
{
    for (ReachableNode& discoveredNode : discoveredNodes)
    {
        if (discoveredNode.node == node)
        {
            return &discoveredNode;
        }
    }
    return nullptr;
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
    , foundDestination(path.empty() ? start : path.back())
    , bIsIntendedDestinationUnreachable(bIsIntendedDestinationUnreachable)
    , bIsPartialPath(bIsPartialPath)
{
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
        const Hints hints)
{
    return Pathfinder(start, goal, map, passabilityChecker, context, hints).getRoute();
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

std::optional<std::deque<MapNode>> Context::getCachedPath(const MapNode& start, const MapNode& goal) const
{
    if (!isCacheEnabled)
    {
        return {};
    }

    if (pathfindingAttempts == 1)
    {
        // Don't look up cached paths on the first pathfinding attempt within a context;
        // the cache is only relevant for subsequent attempts
        return {};
    }

    const auto iter = cachedPaths.find({ start, goal });
    if (iter == cachedPaths.cend())
    {
        return {};
    }
    else
    {
        return { iter->second };
    }
}

std::optional<float> Context::getCachedPathCost(const MapNode& start, const MapNode& goal) const
{
    if (!isCacheEnabled)
    {
        return {};
    }

    if (pathfindingAttempts == 1)
    {
        // Don't look up cached paths on the first pathfinding attempt within a context;
        // the cache is only relevant for subsequent attempts
        return {};
    }

    const auto iter = cachedPathCosts.find({ start, goal });
    if (iter == cachedPathCosts.cend())
    {
        return {};
    }
    else
    {
        return { iter->second };
    }
}

void Context::cachePath(const MapNode& start, const MapNode& goal, const std::deque<MapNode>& path, float pathCost)
{
    if (!isCacheEnabled)
    {
        return;
    }

    const auto key = std::make_pair(start, goal);
    const auto iter = cachedPaths.find(key);
    if (iter == cachedPaths.cend())
    {
        // No cached path found - let's add one
        cachedPaths.emplace(key, path);
        cachedPathCosts.emplace(key, pathCost);
    }
    // TODO: Should we replace already-cached paths if the new cost is lower?
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
