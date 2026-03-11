#include "game/MapUtils.h"

#include <algorithm>  // std::min
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
    MapNode neighbour;
    const bool isUpper = isUpperTile(node.x);

    switch (dir)
    {
    case Facing::South:
        neighbour = { node.x, node.y + 1 };
        break;
    case Facing::SouthWest:
        if (isUpper)
        {
            neighbour = { node.x - 1, node.y };
        }
        else
        {
            neighbour = { node.x - 1, node.y + 1 };
        }
        break;
    case Facing::West:
        neighbour = { node.x - eastWestTileSpan, node.y };
        break;
    case Facing::NorthWest:
        if (isUpper)
        {
            neighbour = { node.x - 1, node.y - 1 };
        }
        else
        {
            neighbour = { node.x - 1, node.y };
        }
        break;
    case Facing::North:
        neighbour = { node.x, node.y - 1 };
        break;
    case Facing::NorthEast:
        if (isUpper)
        {
            neighbour = { node.x + 1, node.y - 1 };
        }
        else
        {
            neighbour = { node.x + 1, node.y };
        }
        break;
    case Facing::East:
        neighbour = { node.x + eastWestTileSpan, node.y };
        break;
    case Facing::SouthEast:
        if (isUpper)
        {
            neighbour = { node.x + 1, node.y };
        }
        else
        {
            neighbour = { node.x + 1, node.y + 1 };
        }
        break;
    default:
        throw std::runtime_error("Invalid facing");
    }

    if (neighbour.x >= 0 && neighbour.y >= 0 && neighbour.x < area.getWidth() && neighbour.y < area.getHeight())
    {
        return neighbour;
    }

    return {};
}

bool isNeighbor(const MapNode& a, const MapNode& b)
{
    const int dist = getTileDistance(a, b);
    if (dist == eastWestTileSpan)
    {
        // East-west tiles are still neighbours even though they have a distance of 2
        return a.y == b.y;
    }

    return dist == 1;
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

int getTileDistance(const MapNode& a, const MapNode& b)
{
    /*
     * Distance calculations are weird due to the zigzag tile layout.
     * Check the unit tests for some examples based on real data.
     */

    // First estimate the number of tiles that we traverse in each axis.
    // This won't be correct, but it's a good starting point.
    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);

    // How many tiles would we cover if this entire movement was diagonal?
    // Diagonal movement traverses 2 tiles from each row.
    int maxDiagonalTiles = dy * 2;

    // Special case for "short journeys".
    if (dy > 0)
    {
        // If the topmost tile is a LOWER tile, and the bottom-most tile is an UPPER tile, we have less far to travel.
        // For example, in these cases the distance differs even though both have dy=1:
        //   (1, 1) -> (2, 2) = distance 1 (short journey)
        //   (2, 1) -> (4, 2) = distance 2
        MapNode top = a;
        MapNode bottom = b;
        if (a.y > b.y)
        {
            std::swap(top, bottom);
        }

        if (isLowerTile(top.x) && isUpperTile(bottom.x))
        {
            // This is a "short journey", so we have less far to travel
            --maxDiagonalTiles;
            --dy;
        }
    }

    // Start by moving as far as we can diagonally.
    // Diagonal movement always spans 1 tile in the x-axis, so we can't move further than dx without overshooting the
    // destination.
    int numTilesMoved = std::min(dx, maxDiagonalTiles);
    dx -= numTilesMoved;
    dy -= numTilesMoved / 2;

    // Whatever's left must be covered by straight lines
    if (dy > 0)
    {
        numTilesMoved += dy;
    }
    if (dx > 0)
    {
        // We can cover 2 tiles with each horizontal movement.
        // Round up because we might need 1 final diagonal movement to change between upper/lower tiles.
        numTilesMoved += ceilInt(static_cast<float>(dx) / eastWestTileSpan);
    }

    return numTilesMoved;
}

int getLogicalDistance(const MapNode& a, const MapNode& b)
{
    /*
     * This is equivalent to getTileDistance, except where commented.
     * This might not be entirely correct right now, but we can revisit it later.
     * At the very least it should give us an approximation of the original game's behavior.
     */

    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);

    int maxDiagonalTiles = dy * 2;

    if (dy > 0)
    {
        MapNode top = a;
        MapNode bottom = b;
        if (a.y > b.y)
        {
            std::swap(top, bottom);
        }

        if (isLowerTile(top.x) && isUpperTile(bottom.x))
        {
            --maxDiagonalTiles;
            --dy;
        }
    }

    int numTilesMoved = std::min(dx, maxDiagonalTiles);
    dx -= numTilesMoved;
    dy -= numTilesMoved / 2;

    if (dy > 0)
    {
        // Vertical movement is treated as double distance
        numTilesMoved += dy * 2;
    }
    if (dx > 0)
    {
        // Horizontal movement is treated as double distance
        numTilesMoved += ceilInt((2.f * dx) / eastWestTileSpan);
    }

    return numTilesMoved;
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
