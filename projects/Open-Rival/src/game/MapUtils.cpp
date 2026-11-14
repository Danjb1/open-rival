#include "game/MapUtils.h"

#include <algorithm>  // std::max, std::min
#include <array>
#include <cstdlib>  // std::abs
#include <optional>
#include <stdexcept>

#include "gfx/RenderConstants.h"

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

/**
 * All directions, in the order in which neighbors are returned by `findNeighbors`.
 *
 * The order itself is arbitrary, but it should not be changed lightly, since it affects which route the pathfinder
 * picks when several routes have the same cost.
 */
constexpr std::array<Facing, maxNeighbors> allDirections = {
    Facing::North,
    Facing::East,
    Facing::South,
    Facing::West,
    Facing::NorthEast,
    Facing::NorthWest,
    Facing::SouthEast,
    Facing::SouthWest,
};

/**
 * Gets the direction that points most directly at a (potentially distant) target, as seen on screen.
 *
 * `dx` and `dy` are the offset to the target, in columns and half-rows.
 */
Facing getScreenDir(int dx, int dy)
{
    /*
     * The target lies in one quadrant, so there are 3 candidates: horizontal (east/west), vertical (north/south), or
     * the diagonal in between. We find the one at the smallest angle to the target, by comparing the cosines of the
     * angles (using the dot product):
     *
     *     cos(angle to horizontal) = targetX / |target|
     *     cos(angle to vertical)   = targetY / |target|
     *     cos(angle to diagonal)   = (targetX * diagonalX + targetY * diagonalY) / (|target| * |diagonal|)
     *
     * To avoid square roots (and floating-point arithmetic), we multiply out the denominators and square both sides
     * of each comparison. All values are positive, so this doesn't affect the result.
     */

    // Distance to the target on screen, in pixels
    constexpr long long columnWidthPx = RenderUtils::tileWidthPx / 2;
    constexpr long long halfRowHeightPx = RenderUtils::tileHeightPx / 2;
    const long long targetX = std::abs(dx) * columnWidthPx;
    const long long targetY = std::abs(dy) * halfRowHeightPx;

    // A diagonal movement covers 1 column and 1 half-row
    constexpr long long diagonalX = columnWidthPx;
    constexpr long long diagonalY = halfRowHeightPx;
    constexpr long long diagonalLengthSquared = diagonalX * diagonalX + diagonalY * diagonalY;

    const long long diagonalAlignment = targetX * diagonalX + targetY * diagonalY;
    const long long diagonalAlignmentSquared = diagonalAlignment * diagonalAlignment;

    const bool isEast = dx > 0;
    const bool isSouth = dy > 0;

    if (targetX * targetX * diagonalLengthSquared > diagonalAlignmentSquared)
    {
        // Closer to horizontal than diagonal
        return isEast ? Facing::East : Facing::West;
    }

    if (targetY * targetY * diagonalLengthSquared > diagonalAlignmentSquared)
    {
        // Closer to vertical than diagonal
        return isSouth ? Facing::South : Facing::North;
    }

    if (isSouth)
    {
        return isEast ? Facing::SouthEast : Facing::SouthWest;
    }
    return isEast ? Facing::NorthEast : Facing::NorthWest;
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
    std::vector<MapNode> neighbors;
    neighbors.reserve(maxNeighbors);

    for (Facing dir : allDirections)
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
    const int dx = to.x - from.x;
    const int dy = getHalfRow(to) - getHalfRow(from);

    if (dx == 0 && dy == 0)
    {
        return Facing::South;
    }

    // Neighboring tiles are exactly 1 movement apart, so we can just find the matching movement. This is the most
    // common case (e.g. during movement), so it's worth skipping the more expensive calculation.
    for (Facing dir : allDirections)
    {
        const Step step = getStep(dir);
        if (step.columns == dx && step.halfRows == dy)
        {
            return dir;
        }
    }

    // Otherwise, find the direction that points most directly at the target
    return getScreenDir(dx, dy);
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
