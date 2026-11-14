#include "game/MapUtils.h"

#include <algorithm>  // std::max, std::min
#include <array>
#include <cstdlib>  // std::abs
#include <optional>
#include <stdexcept>

namespace Rival {

constexpr MapNode MapNode::Invalid { -1, -1 };

namespace MapUtils {

namespace {

/** How far a single movement travels, in columns and half-rows (see `getHalfRow`). */
struct Step
{
    int columns;
    int halfRows;
};

/** Gets the movement made by a single step in the given direction. */
Step getStep(Facing dir)
{
    switch (dir)
    {
    case Facing::South:
        return { 0, 2 };
    case Facing::SouthWest:
        return { -1, 1 };
    case Facing::West:
        return { -eastWestTileSpan, 0 };
    case Facing::NorthWest:
        return { -1, -1 };
    case Facing::North:
        return { 0, -2 };
    case Facing::NorthEast:
        return { 1, -1 };
    case Facing::East:
        return { eastWestTileSpan, 0 };
    case Facing::SouthEast:
        return { 1, 1 };
    }

    throw std::runtime_error("Invalid facing");
}

}  // namespace

int getHalfRow(const MapNode& node)
{
    return 2 * node.y + (isLowerTile(node.x) ? 1 : 0);
}

MapNode fromHalfRow(int x, int halfRow)
{
    return { x, (halfRow - (isLowerTile(x) ? 1 : 0)) / 2 };
}

TileOffset getTileOffset(const MapNode& a, const MapNode& b)
{
    return { std::abs(a.x - b.x), std::abs(getHalfRow(a) - getHalfRow(b)) };
}

std::vector<MapNode> findNeighbors(const MapNode& node, const MapBounds& area)
{
    // Neighbors are always returned in this order. The order itself is arbitrary, but it should not be changed lightly,
    // since it affects which route the pathfinder picks when several routes have the same cost.
    static constexpr std::array<Facing, maxNeighbors> directions = {
        Facing::North,
        Facing::East,
        Facing::South,
        Facing::West,
        Facing::NorthEast,
        Facing::NorthWest,
        Facing::SouthEast,
        Facing::SouthWest,
    };

    std::vector<MapNode> neighbors;
    neighbors.reserve(maxNeighbors);

    for (Facing dir : directions)
    {
        if (std::optional<MapNode> neighbor = getNeighbor(node, dir, area))
        {
            neighbors.push_back(*neighbor);
        }
    }

    return neighbors;
}

std::optional<MapNode> getNeighbor(const MapNode& node, Facing dir, const MapBounds& area)
{
    const MapNode neighbor = moveInDirection(node, dir, 1);

    if (neighbor.x >= 0 && neighbor.y >= 0 && neighbor.x < area.getWidth() && neighbor.y < area.getHeight())
    {
        return neighbor;
    }

    return {};
}

bool isNeighbor(const MapNode& a, const MapNode& b)
{
    return getTileDistance(a, b) == 1;
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

int getTileDistance(const MapNode& a, const MapNode& b)
{
    // Every movement covers a total of 2 columns and/or half-rows (see TileOffset), so the fewest movements needed is
    // half the total. This total is always even, so the division is exact.
    const TileOffset offset = getTileOffset(a, b);
    return (offset.columns + offset.halfRows) / 2;
}

int getLogicalDistance(const MapNode& a, const MapNode& b)
{
    /*
     * Logical distance counts each diagonal movement as 1, and each north/south or east/west movement as 2.
     *
     * This ends up being the higher of columns and half-rows travelled. This is because the shorter of the 2 axes
     * can be covered "for free" by diagonal movement while we cover the longer one.
     *
     * For example, to travel 5 columns and 3 half-rows, we can move diagonally 3 times (distance 3), which leaves 2
     * columns, then move east once (distance 2). The total is 5: the larger of the two values.
     */
    const TileOffset offset = getTileOffset(a, b);
    return std::max(offset.columns, offset.halfRows);
}

MapNode moveInDirection(const MapNode& src, Facing dir, int numMoves)
{
    const Step step = getStep(dir);
    return fromHalfRow(src.x + numMoves * step.columns, getHalfRow(src) + numMoves * step.halfRows);
}

}  // namespace MapUtils
}  // namespace Rival
