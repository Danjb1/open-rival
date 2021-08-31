#include "pch.h"
#include "Pathfinding.h"

#include <algorithm>  // min, reverse
#include <iterator>   // back_inserter
#include <limits>     // numeric_limits
#include <unordered_map>

namespace Rival {
namespace Pathfinding {

    /**
     * A node with an associated score for pathfinding.
     */
    struct ReachableNode {
        Node node;

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
        int cost;

        bool operator<(const ReachableNode& other) const {
            return cost < other.cost;
        }

        bool operator>(const ReachableNode& other) const {
            return other < *this;
        }
    };

    /**
     * Temporary object used in pathfinding.
     *
     * For now this uses a simple A* search. Later, we could investigate the use
     * of waypoints if this does not perform adequately.
     */
    class Pathfinder {
    public:
        Pathfinder::Pathfinder(
                Node start, Node goal, const PathfindingMap& map);

        std::vector<Node> getPath() const { return pathFound; }

    private:
        /**
         * The starting node.
         */
        Node start;

        /**
         * The destination node.
         */
        Node goal;

        /**
         * The PathfindingMap used to find obstacles, etc.
         */
        const PathfindingMap& map;

        /**
         * All discovered nodes, sorted with the "best" nodes first.
         */
        std::vector<ReachableNode> discoveredNodes;

        /**
         * Map of node -> lowest cost to reach that node from the start.
         */
        std::unordered_map<Node, int> costToNode;

        /**
         * Map of node -> previous node in the shortest path found.
         */
        std::unordered_map<Node, Node> prevNode;

        /**
         * After construction, contains the shortest path found.
         */
        std::vector<Node> pathFound;

        std::vector<Node> findPath();
        bool isFinished() const;
        ReachableNode popBestNode();
        int estimateCostToGoal(const Node& node) const;
        std::vector<Node> reconstructPath(const Node& node) const;
        std::vector<Node> findNeighbors(const Node& node) const;
        bool isNodeTraversable(const Node& node) const;
        int getCostToNode(const Node& node) const;
        void updatePathToNode(const Node& node, int newCost);
        ReachableNode* findDiscoveredNode(const Node& node);
    };

    /**
     * Constructs a Pathfinder which attempts to find a path connecting start
     * to goal.
     */
    Pathfinder::Pathfinder(Node start, Node goal, const PathfindingMap& map)
        : start(start),
          goal(goal),
          map(map),
          pathFound(findPath()) {}

    /**
     * Attempts to find a path based on the Pathfinder's configuration.
     */
    std::vector<Node> Pathfinder::findPath() {
        discoveredNodes.push_back({ start, 0 });
        costToNode[start] = 0;

        while (!isFinished()) {
            ReachableNode current = popBestNode();

            // See if we've reached the goal
            if (current.node == goal) {
                return reconstructPath(current.node);
            }

            std::vector<Node> neighbors = findNeighbors(current.node);

            for (Node neighbor : neighbors) {
                int newCostToNeighbor = getCostToNode(current.node) + 1;
                if (newCostToNeighbor < getCostToNode(neighbor)) {
                    // This path to neighbor is better than any previous one
                    costToNode[neighbor] = newCostToNeighbor;
                    prevNode[neighbor] = current.node;
                    updatePathToNode(neighbor, newCostToNeighbor);
                }
            }
        }

        // The goal could not be reached
        return {};
    }

    bool Pathfinder::isFinished() const {
        return discoveredNodes.empty();
    }

    /**
     * Removes the ReachableNode with the lowest estimated cost from the list
     * of discovered Nodes, and returns it.
     */
    ReachableNode Pathfinder::popBestNode() {
        // Making `discoveredNodes` into a min-heap means we can extract the
        // best node in O(1) time.
        std::make_heap(
                discoveredNodes.begin(),
                discoveredNodes.end(),
                std::greater<ReachableNode> {});
        // After calling `pop_heap`, the best node is at the back of the list
        std::pop_heap(
                discoveredNodes.begin(),
                discoveredNodes.end(),
                std::greater<ReachableNode> {});
        ReachableNode bestNode = discoveredNodes.back();
        discoveredNodes.pop_back();
        return bestNode;
    }

    /**
     * Heuristic function used to estimate the cost from a Node to the goal.
     */
    int Pathfinder::estimateCostToGoal(const Node& node) const {
        if (node == goal) {
            return 0;
        }

        int dx = abs(node.x - goal.x);
        int dy = abs(node.y - goal.y);

        // Whatever distance x and y have in common can be covered diagonally
        int diagonalDistance = std::min(dx, dy);
        int remainingDistance = abs(dx - dy);

        return diagonalDistance + remainingDistance;
    }

    /**
     * Returns the path found from the start to the given Node.
     */
    std::vector<Node> Pathfinder::reconstructPath(const Node& node) const {
        std::vector<Node> path = { node };
        Node currentNode = node;

        // Follow the previous nodes back to the start
        while (currentNode != start) {
            auto it = prevNode.find(currentNode);
            if (it == prevNode.end()) {
                // No previous node found. This should never happen since we
                // don't enter the loop for the start node.
                break;
            } else {
                currentNode = it->second;
                path.push_back(currentNode);
            }
        }

        // We have a path from the end to the start, but we want the opposite
        std::reverse(path.begin(), path.end());

        return path;
    }

    /**
     * Returns a vector containing all valid neighbors of the given Node.
     */
    std::vector<Node> Pathfinder::findNeighbors(const Node& node) const {
        std::vector<Node> allNeighbors;

        // First determine which map locations are valid relative to this node.
        // The neighborhood is very strange due to the zigzag nature of the map.
        // A direct move east or west actually moves 2 tiles.
        bool hasNorth = node.y > 0;
        bool hasSouth = node.y < map.getHeight() - 1;
        bool hasEast = node.x > 1;
        bool hasWest = node.x < map.getWidth() - 2;

        // Find all valid neighbors
        if (hasNorth) {
            allNeighbors.push_back({ node.x, node.y - 1 });
        }
        if (hasEast) {
            allNeighbors.push_back({ node.x + 2, node.y });
        }
        if (hasSouth) {
            allNeighbors.push_back({ node.x, node.y + 1 });
        }
        if (hasWest) {
            allNeighbors.push_back({ node.x - 2, node.y });
        }

        // The diagonal neighbors depend on which part of the zigzag we are in
        bool hasDiagonalEast = node.x < map.getWidth() - 1;
        bool hasDiagonalWest = node.x > 0;
        if (node.x % 2 == 0) {
            // We are in the top part of the zigzag;
            // => Moving diagonally north moves us into the row above.
            // => Moving diagonally south keeps us in the same row.
            bool hasNorthEast = hasDiagonalEast && hasNorth;
            bool hasNorthWest = hasDiagonalWest && hasNorth;
            bool hasSouthEast = hasDiagonalEast;
            bool hasSouthWest = hasDiagonalWest;

            if (hasNorthEast) {
                allNeighbors.push_back({ node.x + 1, node.y - 1 });
            }
            if (hasNorthWest) {
                allNeighbors.push_back({ node.x - 1, node.y - 1 });
            }
            if (hasSouthEast) {
                allNeighbors.push_back({ node.x + 1, node.y });
            }
            if (hasSouthWest) {
                allNeighbors.push_back({ node.x - 1, node.y });
            }

        } else {
            // We are in the bottom part of the zigzag;
            // => Moving diagonally north keeps us in the same row
            // => Moving diagonally south moves us into the row below.
            bool hasNorthEast = hasDiagonalEast;
            bool hasNorthWest = hasDiagonalWest;
            bool hasSouthEast = hasDiagonalEast && hasSouth;
            bool hasSouthWest = hasDiagonalWest && hasSouth;

            if (hasNorthEast) {
                allNeighbors.push_back({ node.x + 1, node.y });
            }
            if (hasNorthWest) {
                allNeighbors.push_back({ node.x - 1, node.y });
            }
            if (hasSouthEast) {
                allNeighbors.push_back({ node.x + 1, node.y + 1 });
            }
            if (hasSouthWest) {
                allNeighbors.push_back({ node.x - 1, node.y + 1 });
            }
        }

        // Filter out non-traversable neighbors
        std::vector<Node> validNeighbors;
        std::copy_if(allNeighbors.begin(),
                allNeighbors.end(),
                std::back_inserter(validNeighbors),
                [this](Node n) { return this->isNodeTraversable(n); });

        return validNeighbors;
    }

    /**
     * Determines if a Node is a traversable tile.
     */
    bool Pathfinder::isNodeTraversable(const Node& node) const {
        return map.getPassability(node.x, node.y) == TilePassability::Clear;
    }

    /**
     * Gets the cost of moving from the start to the given Node.
     *
     * Returns the integer max if no path has been found yet.
     */
    int Pathfinder::getCostToNode(const Node& node) const {
        auto it = costToNode.find(node);
        if (it == costToNode.end()) {
            // No path to node found yet
            return std::numeric_limits<int>::max();
        }
        return it->second;
    }

    /**
     * Updates the path to a Node with a shorter one, or adds a new path to the
     * Node if this is the first one found.
     */
    void Pathfinder::updatePathToNode(const Node& node, int newCost) {
        int newEstimate = newCost + estimateCostToGoal(node);
        ReachableNode* existingNode = findDiscoveredNode(node);
        if (existingNode) {
            existingNode->cost = newEstimate;
        } else {
            discoveredNodes.push_back({ node, newEstimate });
        }
    }

    /**
     * Finds the ReachableNode associated with the given Node, if present.
     */
    ReachableNode* Pathfinder::findDiscoveredNode(const Node& node) {
        for (ReachableNode& discoveredNode : discoveredNodes) {
            if (discoveredNode.node == node) {
                return &discoveredNode;
            }
        }
        return nullptr;
    }

    std::vector<Node> findPath(
            Node start,
            Node goal,
            const PathfindingMap& map) {
        return Pathfinder(start, goal, map).getPath();
    }

}}  // namespace Rival::Pathfinding
