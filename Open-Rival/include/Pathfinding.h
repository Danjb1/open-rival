#ifndef PATHFINDING_UTILS_H
#define PATHFINDING_UTILS_H

#include <deque>

#include "MapUtils.h"
#include "Scenario.h"

namespace Rival {
namespace Pathfinding {

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
        Route(MapNode destination, std::deque<MapNode> path);

        /**
         * Determines if this Route is empty.
         */
        bool isEmpty() const;

        MapNode getDestination() const { return destination; }

        /**
         * Removes the next MapNode from the path and returns it.
         */
        MapNode pop();

    private:
        MapNode destination;
        std::deque<MapNode> path;
    };

    /**
     * Attempts to find the optimal path connecting `start` to `goal`.
     */
    Route findPath(MapNode start, MapNode goal, const PathfindingMap& map);

}}  // namespace Rival::Pathfinding

namespace std {

    /**
     * Custom hash function for MapNode.
     */
    template <>
    struct hash<Rival::MapNode> {
        std::size_t operator()(
                Rival::MapNode const& node) const noexcept {
            // Given that x/y will never exceed 16 bits, this ought to give a
            // totally unique value for each node.
            return node.x | (node.y << 16);
        }
    };

}  // namespace std

#endif  // PATHFINDING_UTILS_H
