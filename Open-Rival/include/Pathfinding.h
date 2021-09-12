#ifndef PATHFINDING_UTILS_H
#define PATHFINDING_UTILS_H

#include <deque>

#include "MapUtils.h"
#include "Scenario.h"

namespace Rival {
namespace Pathfinding {

    /**
     * Interface used to determine if a MapNode is traversable.
     */
    class PassabilityChecker {
    public:
        virtual bool isNodeTraversable(
                const PathfindingMap& map, const MapNode& node) const = 0;
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
    Route findPath(MapNode start,
            MapNode goal,
            const PathfindingMap& map,
            const PassabilityChecker& passabilityChecker);

}}  // namespace Rival::Pathfinding

#endif  // PATHFINDING_UTILS_H
