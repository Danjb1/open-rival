#include "game/MapUtils.h"

#include <algorithm>  // std::max, std::min
#include <cstdlib>    // std::abs
#include <stdexcept>

namespace Rival {

constexpr MapNode MapNode::Invalid { -1, -1 };

namespace MapUtils {

std::vector<MapNode> findNeighbors(const MapNode& node, const MapBounds& area)
{
    std::vector<MapNode> neighbors;
    neighbors.reserve(maxNeighbors);

    // First determine which map locations are valid relative to this node.
    // The neighborhood is very strange due to the zigzag nature of the map.
    // A direct move east or west actually moves 2 tiles.
    const bool hasNorth = node.y > 0;
    const bool hasSouth = node.y < area.getHeight() - 1;
    const bool hasEast = node.x < area.getWidth() - eastWestTileSpan;
    const bool hasWest = node.x >= eastWestTileSpan;

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
    const bool hasDiagonalEast = node.x < area.getWidth() - 1;
    const bool hasDiagonalWest = node.x > 0;
    if (isUpperTile(node.x))
    {
        // We are in the top part of the zigzag;
        // => Moving diagonally north moves us into the row above.
        // => Moving diagonally south keeps us in the same row.
        const bool hasNorthEast = hasDiagonalEast && hasNorth;
        const bool hasNorthWest = hasDiagonalWest && hasNorth;
        const bool hasSouthEast = hasDiagonalEast;
        const bool hasSouthWest = hasDiagonalWest;

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
        const bool hasNorthEast = hasDiagonalEast;
        const bool hasNorthWest = hasDiagonalWest;
        const bool hasSouthEast = hasDiagonalEast && hasSouth;
        const bool hasSouthWest = hasDiagonalWest && hasSouth;

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

std::optional<MapNode> getNeighbor(const MapNode& node, Facing dir, const MapBounds& area)
{
    MapNode neighbor;
    const bool isUpper = isUpperTile(node.x);

    switch (dir)
    {
    case Facing::South:
        neighbor = { node.x, node.y + 1 };
        break;
    case Facing::SouthWest:
        if (isUpper)
        {
            neighbor = { node.x - 1, node.y };
        }
        else
        {
            neighbor = { node.x - 1, node.y + 1 };
        }
        break;
    case Facing::West:
        neighbor = { node.x - eastWestTileSpan, node.y };
        break;
    case Facing::NorthWest:
        if (isUpper)
        {
            neighbor = { node.x - 1, node.y - 1 };
        }
        else
        {
            neighbor = { node.x - 1, node.y };
        }
        break;
    case Facing::North:
        neighbor = { node.x, node.y - 1 };
        break;
    case Facing::NorthEast:
        if (isUpper)
        {
            neighbor = { node.x + 1, node.y - 1 };
        }
        else
        {
            neighbor = { node.x + 1, node.y };
        }
        break;
    case Facing::East:
        neighbor = { node.x + eastWestTileSpan, node.y };
        break;
    case Facing::SouthEast:
        if (isUpper)
        {
            neighbor = { node.x + 1, node.y };
        }
        else
        {
            neighbor = { node.x + 1, node.y + 1 };
        }
        break;
    default:
        throw std::runtime_error("Invalid facing");
    }

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

static int ceilInt(float x)
{
    return static_cast<int>(std::ceil(x));
}

TileOffset getTileOffset(const MapNode& a, const MapNode& b)
{
    auto toHalfRows = [](const MapNode& node) { return 2 * node.y + (isLowerTile(node.x) ? 1 : 0); };
    return { std::abs(a.x - b.x), std::abs(toHalfRows(a) - toHalfRows(b)) };
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

MapNode addLogicalDistance(const MapNode& src, Facing dir, int distance)
{
    /*
     * The cardinal directions are straightforward, but the diagonals are a bit more complicated.
     * Essentially, when travelling diagonally, you traverse 2 tiles in each row, so you cover half the vertical
     * distance. We also have to adjust depending on whether you start in an upper or lower tile, as this determines
     * when you first change rows.
     *
     * For example, if you are in a lower row and travelling 1 tile northeast, you transition to an upper row but your
     * y co-ordinate does not actually change.
     */
    switch (dir)
    {
    case Facing::North:
        return { src.x, src.y - distance };
    case Facing::East:
        return { src.x + eastWestTileSpan * distance, src.y };
    case Facing::South:
        return { src.x, src.y + distance };
    case Facing::West:
        return { src.x - eastWestTileSpan * distance, src.y };

    case Facing::NorthWest: {
        int adjustedDist = isLowerTile(src.x) ? distance - 1 : distance;
        return { src.x - distance, src.y - ceilInt(adjustedDist / 2.f) };
    }
    case Facing::NorthEast: {
        int adjustedDist = isLowerTile(src.x) ? distance - 1 : distance;
        return { src.x + distance, src.y - ceilInt(adjustedDist / 2.f) };
    }
    case Facing::SouthEast: {
        int adjustedDist = isUpperTile(src.x) ? distance - 1 : distance;
        return { src.x + distance, src.y + ceilInt(adjustedDist / 2.f) };
    }
    case Facing::SouthWest: {
        int adjustedDist = isUpperTile(src.x) ? distance - 1 : distance;
        return { src.x - distance, src.y + ceilInt(adjustedDist / 2.f) };
    }
    }

    throw std::runtime_error("Direction not supported!");
}

}  // namespace MapUtils
}  // namespace Rival
