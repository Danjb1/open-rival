#ifndef MAP_UTILS_H
#define MAP_UTILS_H

#include <vector>

#include "Scenario.h"

namespace Rival {

    /**
     * The co-ordinates of a tile.
     */
    struct MapNode {
        int x;
        int y;

        bool operator==(const MapNode& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const MapNode& other) const {
            return !(*this == other);
        }
    };

    namespace MapUtils {

        /**
         * Finds all valid neighbors of the given MapNode.
         */
        std::vector<MapNode> findNeighbors(MapNode node, const GameArea& area);

    }  // namespace MapUtils
}  // namespace Rival

#endif  // MAP_UTILS_H
