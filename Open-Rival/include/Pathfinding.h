#ifndef PATHFINDING_UTILS_H
#define PATHFINDING_UTILS_H

#include <deque>

#include "Scenario.h"

namespace Rival {
namespace Pathfinding {

    /**
     * A node representing a point on a path.
     */
    struct Node {
        int x;
        int y;

        bool operator==(const Node& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Node& other) const {
            return !(*this == other);
        }
    };

    /**
     * A planned path to a destination.
     */
    class Route {
    public:
        /**
         * Default constructor; makes a Route with an empty path.
         */
        Route();

        /**
         * Constructs a Route with a path and destination.
         */
        Route(Node destination, std::deque<Node> path);

        /**
         * Determines if this Route is empty.
         */
        bool isEmpty() const;

        Node getDestination() const { return destination; }

        /**
         * Removes the next Node from the path and returns it.
         */
        Node pop();

    private:
        Node destination;
        std::deque<Node> path;
    };

    /**
     * Attempts to find the optimal path connecting `start` to `goal`.
     */
    Route findPath(Node start, Node goal, const PathfindingMap& map);

}}  // namespace Rival::Pathfinding

namespace std {

    /**
     * Custom hash function for Node.
     */
    template <>
    struct hash<Rival::Pathfinding::Node> {
        std::size_t operator()(
                Rival::Pathfinding::Node const& node) const noexcept {
            // Given that x/y will never exceed 16 bits, this ought to give a
            // totally unique value for each node.
            return node.x | (node.y << 16);
        }
    };

}  // namespace std

#endif  // PATHFINDING_UTILS_H
