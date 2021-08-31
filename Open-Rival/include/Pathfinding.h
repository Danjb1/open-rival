#ifndef PATHFINDING_UTILS_H
#define PATHFINDING_UTILS_H

#include <vector>

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
     * Attempts to find the optimal path connecting `start` to `goal`.
     */
    std::vector<Node> findPath(
            Node start, Node goal, const PathfindingMap& map);

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
