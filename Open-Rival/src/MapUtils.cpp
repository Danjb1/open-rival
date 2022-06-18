#include "pch.h"

#include "MapUtils.h"

namespace Rival { namespace MapUtils {

std::vector<MapNode> findNeighbors(const MapNode& node, const MapBounds& area)
{
    std::vector<MapNode> neighbors;

    // First determine which map locations are valid relative to this node.
    // The neighborhood is very strange due to the zigzag nature of the map.
    // A direct move east or west actually moves 2 tiles.
    bool hasNorth = node.y > 0;
    bool hasSouth = node.y < area.getHeight() - 1;
    bool hasEast = node.x > 1;
    bool hasWest = node.x < area.getWidth() - eastWestTileSpan;

    // Find all valid neighbors
    if (hasNorth)
    {
        neighbors.push_back({ node.x, node.y - 1 });
    }
    if (hasEast)
    {
        neighbors.push_back({ node.x + eastWestTileSpan, node.y });
    }
    if (hasSouth)
    {
        neighbors.push_back({ node.x, node.y + 1 });
    }
    if (hasWest)
    {
        neighbors.push_back({ node.x - eastWestTileSpan, node.y });
    }

    // The diagonal neighbors depend on which part of the zigzag we are in
    bool hasDiagonalEast = node.x < area.getWidth() - 1;
    bool hasDiagonalWest = node.x > 0;
    if (isUpperTile(node.x))
    {
        // We are in the top part of the zigzag;
        // => Moving diagonally north moves us into the row above.
        // => Moving diagonally south keeps us in the same row.
        bool hasNorthEast = hasDiagonalEast && hasNorth;
        bool hasNorthWest = hasDiagonalWest && hasNorth;
        bool hasSouthEast = hasDiagonalEast;
        bool hasSouthWest = hasDiagonalWest;

        if (hasNorthEast)
        {
            neighbors.push_back({ node.x + 1, node.y - 1 });
        }
        if (hasNorthWest)
        {
            neighbors.push_back({ node.x - 1, node.y - 1 });
        }
        if (hasSouthEast)
        {
            neighbors.push_back({ node.x + 1, node.y });
        }
        if (hasSouthWest)
        {
            neighbors.push_back({ node.x - 1, node.y });
        }
    }
    else
    {
        // We are in the bottom part of the zigzag;
        // => Moving diagonally north keeps us in the same row.
        // => Moving diagonally south moves us into the row below.
        bool hasNorthEast = hasDiagonalEast;
        bool hasNorthWest = hasDiagonalWest;
        bool hasSouthEast = hasDiagonalEast && hasSouth;
        bool hasSouthWest = hasDiagonalWest && hasSouth;

        if (hasNorthEast)
        {
            neighbors.push_back({ node.x + 1, node.y });
        }
        if (hasNorthWest)
        {
            neighbors.push_back({ node.x - 1, node.y });
        }
        if (hasSouthEast)
        {
            neighbors.push_back({ node.x + 1, node.y + 1 });
        }
        if (hasSouthWest)
        {
            neighbors.push_back({ node.x - 1, node.y + 1 });
        }
    }

    return neighbors;
}

Facing getDir(const MapNode& from, const MapNode& to)
{
    // Let's get the easy ones out of the way first
    if (from.x == to.x)
    {
        return from.y <= to.y ? Facing::South : Facing::North;
    }

    // Determine x/y distance between the 2 nodes
    int dx = to.x - from.x;
    int dy = to.y - from.y;

    // We need to factor in the zigzagging nature of the rows. Even within
    // the same row, tiles can be positioned above or below each other.
    if (isUpperTile(from.x) && isLowerTile(to.x))
    {
        if (dy < 0)
        {
            // Moving from top part of a zigzag to the row above
            --dy;
        }
        else
        {
            // Moving from top part of a zigzag to the bottom
            ++dy;
        }
    }
    else if (isLowerTile(from.x) && isUpperTile(to.x))
    {
        if (dy <= 0)
        {
            // Moving from bottom part of a zigzag to the top
            --dy;
        }
        else
        {
            // Moving from bottom part of a zigzag to the row below
            ++dy;
        }
    }

    // If the x-delta is significantly more than the y-delta, we should
    // face east/west.
    if (abs(dx) > 2 * abs(dy))
    {
        return dx < 0 ? Facing::West : Facing::East;
    }

    if (isUpperTile(from.x))
    {
        // We are in the top part of the zigzag;
        // => Moving to the row above is diagonally north.
        // => Moving to the *same* row is diagonally south.
        if (dy < 0)
        {
            return dx < 0 ? Facing::NorthWest : Facing::NorthEast;
        }
        else
        {
            return dx < 0 ? Facing::SouthWest : Facing::SouthEast;
        }
    }
    else
    {
        // We are in the bottom part of the zigzag;
        // => Moving to the *same* row is diagonally north.
        // => Moving to the row below is diagonally south.
        if (dy <= 0)
        {
            return dx < 0 ? Facing::NorthWest : Facing::NorthEast;
        }
        else
        {
            return dx < 0 ? Facing ::SouthWest : Facing::SouthEast;
        }
    }
}

}}  // namespace Rival::MapUtils
