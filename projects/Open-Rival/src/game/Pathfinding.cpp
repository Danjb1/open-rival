#include "game/Pathfinding.h"

#include <algorithm>  // min, reverse
#include <iterator>   // back_inserter
#include <limits>     // numeric_limits
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
    Pathfinder(MapNode start, MapNode goal, const PathfindingMap& map, const PassabilityChecker& passabilityChecker);

    Route getRoute() const
    {
        return route;
    }

private:
    std::deque<MapNode> findPath();
    bool isFinished() const;
    ReachableNode popBestNode();
    float estimateCostToGoal(const MapNode& node);
    std::deque<MapNode> reconstructPath(const MapNode& node) const;
    std::vector<MapNode> findNeighbors(const MapNode& node) const;
    float getCostToNode(const MapNode& node) const;
    float getMovementCost(const MapNode& from, const MapNode& to) const;
    void updatePathToNode(const MapNode& node, float newCost);
    ReachableNode* findDiscoveredNode(const MapNode& node);

private:
    /** Base movement cost for moving between two tiles. */
    static constexpr float baseMovementCost = 1.f;

    /**
     * Movement cost when trying to move into a tile that is currently obstructed.
     * Note that when moving a group, obstructions at the destination are inevitable.
     *
     * This is tricky to get right:
     * - If this is too low, units will not be able to pathfind around obstructions in cases where the alternative route
     *   is long.
     * - If this is too high, group movement becomes computationally expensive because units will waste their time
     *   trying long alternative routes to avoid (inevitable) obstructions.
     */
    static constexpr float obstructedMovementCost = 3.f;

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

    /** Maximum nodes that the pathfinder can visit before giving up.
     * This is more of a safety mechanism than anything, and should not be an issue unless pathfinding across vast
     * distances. */
    static constexpr int maxNodesVisited = 50000;

    /** The starting node. */
    MapNode start;

    /** The destination node. */
    MapNode goal;

    /** The PathfindingMap used to find obstacles, etc. */
    const PathfindingMap& map;

    /** Object used to check for passability. */
    const PassabilityChecker& passabilityChecker;

    /** All discovered nodes, sorted with the "best" nodes first. */
    std::vector<ReachableNode> discoveredNodes;

    /** Map of node -> lowest cost to reach that node from the start. */
    std::unordered_map<MapNode, float> costToNode;

    /** Map of node -> estimated cost to reach the goal. */
    std::unordered_map<MapNode, float> cachedCostToGoal;

    /** Map of node -> previous node in the shortest path found. */
    std::unordered_map<MapNode, MapNode> prevNode;

    /** After construction, contains the shortest route to the goal. */
    Route route;

    /** The lowest cost found to the goal. */
    float lowestCostToGoal = std::numeric_limits<float>::max();

    /** The closest node found to the goal. */
    MapNode closestNodeToGoal;

    /** Nodes visited during pathfinding. */
    int nodesVisited = 0;
};

/**
 * Constructs a Pathfinder which attempts to find a path connecting start to goal.
 */
Pathfinder::Pathfinder(
        MapNode start, MapNode goal, const PathfindingMap& map, const PassabilityChecker& passabilityChecker)
    : start(start)
    , goal(goal)
    , map(map)
    , passabilityChecker(passabilityChecker)
{
    route = { goal, findPath() };
}

/**
 * Attempts to find a path based on the Pathfinder's configuration.
 */
std::deque<MapNode> Pathfinder::findPath()
{
    if (start == goal)
    {
        return {};
    }

    discoveredNodes.push_back({ start, 0 });
    closestNodeToGoal = start;
    costToNode[start] = 0;

    while (!isFinished())
    {
        if (nodesVisited >= maxNodesVisited)
        {
            LOG_WARN("Pathfinding exceeded maximum nodes visited");
            break;
        }

        const ReachableNode current = popBestNode();

        // See if we've reached the goal
        if (current.node == goal)
        {
            return reconstructPath(current.node);
        }

        std::vector<MapNode> neighbors = findNeighbors(current.node);

        for (const MapNode& neighbor : neighbors)
        {
            ++nodesVisited;
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
    }

    // The goal could not be reached - get as close as we can.
    // Note that, due to the fact that due to the fact that we do not set lowestCostToGoal for the start node,
    // we will always move at least 1 tile (if possible), even if it takes us further away than our starting
    // location. This is in keeping with the behavior of the original game.
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
    if (node == goal)
    {
        return 0.f;
    }

    auto iter = cachedCostToGoal.find(node);
    if (iter != cachedCostToGoal.cend())
    {
        return iter->second;
    }

    const int dx = abs(node.x - goal.x);
    const int dy = abs(node.y - goal.y);

    // Whatever distance x and y have in common can be covered diagonally
    const int diagonalDistance = std::min(dx, dy);
    const int remainingDistance = abs(dx - dy);

    const float costToGoal = static_cast<float>(diagonalDistance + remainingDistance);
    cachedCostToGoal.emplace(node, costToGoal);

    return costToGoal;
}

/**
 * Returns the path found from the start to the given MapNode.
 */
std::deque<MapNode> Pathfinder::reconstructPath(const MapNode& node) const
{
    std::deque<MapNode> path = {};
    MapNode currentNode = node;

    // Follow the previous nodes back to the start
    while (currentNode != start)
    {
        path.push_front(currentNode);
        auto it = prevNode.find(currentNode);
        if (it == prevNode.end())
        {
            // No previous node found. This should never happen since we
            // don't enter the loop for the start node.
            break;
        }
        currentNode = it->second;
    }

    return path;
}

/**
 * Returns a vector containing all valid neighbors of the given MapNode.
 */
std::vector<MapNode> Pathfinder::findNeighbors(const MapNode& node) const
{
    std::vector<MapNode> allNeighbors = MapUtils::findNeighbors(node, map);

    // Filter out non-traversable neighbors
    std::vector<MapNode> validNeighbors;
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
    auto it = costToNode.find(node);
    if (it == costToNode.end())
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
    const bool isObstructed = passabilityChecker.isNodeObstructed(map, to);
    const float movementCost = isObstructed ? obstructedMovementCost : baseMovementCost;

    const Facing movementDir = MapUtils::getDir(from, to);
    const float dirMultiplier =
            (movementDir == Facing::East || movementDir == Facing::West) ? horizontalMoveCostMultiplier : 1.f;

    return movementCost * dirMultiplier;
}

/**
 * Updates the path to a node with a shorter one, or adds a new path to
 * the node if this is the first one found.
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
        lowestCostToGoal = estimatedCostToGoal;
        closestNodeToGoal = node;
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

Route::Route()
    : destination({ 0, 0 })
{
}

Route::Route(MapNode destination, std::deque<MapNode> path)
    : destination(destination)
    , path(path)
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

Route findPath(MapNode start, MapNode goal, const PathfindingMap& map, const PassabilityChecker& passabilityChecker)
{
    return Pathfinder(start, goal, map, passabilityChecker).getRoute();
}

}}  // namespace Rival::Pathfinding
