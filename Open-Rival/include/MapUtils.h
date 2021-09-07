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

    /**
     * A compass direction.
     */
    enum class Facing : std::uint8_t {
        South,
        SouthWest,
        West,
        NorthWest,
        North,
        NorthEast,
        East,
        SouthEast
    };

    namespace MapUtils {

        /**
         * Finds all valid neighbors of the given MapNode.
         */
        std::vector<MapNode> findNeighbors(MapNode node, const MapBounds& area);

        /**
         * Gets the most pertinent direction between 2 neighbouring tiles.
         *
         * For example, if `to` is directly above `from`, this will return
         * `Facing::North`.
         *
         * If the MapNodes are identical, this returns South.
         */
        Facing getDir(MapNode from, MapNode to);

    }  // namespace MapUtils
}  // namespace Rival

#endif  // MAP_UTILS_H
