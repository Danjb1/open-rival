#include "pch.h"
#include "MapUtils.h"

namespace Rival {
namespace MapUtils {

    std::vector<MapNode> findNeighbors(MapNode node, const MapBounds& area) {
        std::vector<MapNode> neighbors;

        // First determine which map locations are valid relative to this node.
        // The neighborhood is very strange due to the zigzag nature of the map.
        // A direct move east or west actually moves 2 tiles.
        bool hasNorth = node.y > 0;
        bool hasSouth = node.y < area.getHeight() - 1;
        bool hasEast = node.x > 1;
        bool hasWest = node.x < area.getWidth() - 2;

        // Find all valid neighbors
        if (hasNorth) {
            neighbors.push_back({ node.x, node.y - 1 });
        }
        if (hasEast) {
            neighbors.push_back({ node.x + 2, node.y });
        }
        if (hasSouth) {
            neighbors.push_back({ node.x, node.y + 1 });
        }
        if (hasWest) {
            neighbors.push_back({ node.x - 2, node.y });
        }

        // The diagonal neighbors depend on which part of the zigzag we are in
        bool hasDiagonalEast = node.x < area.getWidth() - 1;
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
                neighbors.push_back({ node.x + 1, node.y - 1 });
            }
            if (hasNorthWest) {
                neighbors.push_back({ node.x - 1, node.y - 1 });
            }
            if (hasSouthEast) {
                neighbors.push_back({ node.x + 1, node.y });
            }
            if (hasSouthWest) {
                neighbors.push_back({ node.x - 1, node.y });
            }

        } else {
            // We are in the bottom part of the zigzag;
            // => Moving diagonally north keeps us in the same row.
            // => Moving diagonally south moves us into the row below.
            bool hasNorthEast = hasDiagonalEast;
            bool hasNorthWest = hasDiagonalWest;
            bool hasSouthEast = hasDiagonalEast && hasSouth;
            bool hasSouthWest = hasDiagonalWest && hasSouth;

            if (hasNorthEast) {
                neighbors.push_back({ node.x + 1, node.y });
            }
            if (hasNorthWest) {
                neighbors.push_back({ node.x - 1, node.y });
            }
            if (hasSouthEast) {
                neighbors.push_back({ node.x + 1, node.y + 1 });
            }
            if (hasSouthWest) {
                neighbors.push_back({ node.x - 1, node.y + 1 });
            }
        }

        return neighbors;
    }

    Facing getDir(MapNode from, MapNode to) {
        // Let's get the easy ones out of the way first
        if (from.x == to.x) {
            if (from.y < to.y) {
                return Facing::North;
            } else {
                return Facing::South;
            }
        }

        // If the x-delta is significantly more than the y-delta, we should
        // face east/west.
        int dx = from.x - to.x;
        int dy = from.y - to.y;
        if (abs(dx) > 2 * abs(dy)) {
            if (dx < 0) {
                return Facing::West;
            } else {
                return Facing::East;
            }
        }

        if (from.x % 2 == 0) {
            // We are in the top part of the zigzag;
            // => Moving to the row above is diagonally north.
            // => Moving to the *same* row is diagonally south.
            if (dy < 0) {
                if (dx < 0) {
                    return Facing::NorthWest;
                } else {
                    return Facing::NorthEast;
                }
            } else {
                if (dx < 0) {
                    return Facing::SouthWest;
                } else {
                    return Facing::SouthEast;
                }
            }
        } else {
            // We are in the bottom part of the zigzag;
            // => Moving to the *same* row is diagonally north.
            // => Moving to the row below is diagonally north.
            if (dy <= 0) {
                if (dx < 0) {
                    return Facing::NorthWest;
                } else {
                    return Facing::NorthEast;
                }
            } else {
                if (dx < 0) {
                    return Facing::SouthWest;
                } else {
                    return Facing::SouthEast;
                }
            }
        }
    }

}}  // namespace Rival::MapUtils
