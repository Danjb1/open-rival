#include "game/MapUtils.h"

#include <algorithm>  // std::min
#include <stdexcept>

namespace Rival { namespace MapUtils {

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

    switch (dir)
    {
    case Facing::South:
        neighbour = { node.x, node.y + 1 };
        break;
    case Facing::SouthWest:
        neighbour = { node.x - 1, node.y };
        break;
    case Facing::West:
        neighbour = { node.x - eastWestTileSpan, node.y };
        break;
    case Facing::NorthWest:
        neighbour = { node.x - 1, node.y - 1 };
        break;
    case Facing::North:
        neighbour = { node.x, node.y - 1 };
        break;
    case Facing::NorthEast:
        neighbour = { node.x + 1, node.y - 1 };
        break;
    case Facing::East:
        neighbour = { node.x + eastWestTileSpan, node.y };
        break;
    case Facing::SouthEast:
        neighbour = { node.x + 1, node.y };
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
    const int dist = getDistance(a, b);
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

int getDistance(const MapNode& a, const MapNode& b)
{
    // Distance calculations are weird due to the zigzag tile layout.
    // Check the unit tests for some examples based on real data.
    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);

    // Determine if this is a "short journey".
    // If the topmost tile is a LOWER tile, and the bottom-most tile is an UPPER tile, we have less far to travel.
    bool isShortJourney = false;
    if (dy > 0)
    {
        MapNode top = a;
        MapNode bottom = b;
        if (a.y > b.y)
        {
            std::swap(top, bottom);
        }
        const bool topIsLower = isLowerTile(top.x);
        const bool bottomIsUpper = isUpperTile(bottom.x);
        isShortJourney = topIsLower && bottomIsUpper;
    }

    // When travelling diagonally, the y-distance is greater because we traverse up to 2 tiles from each row
    int dy_diag = dy * 2;

    // For short journeys, there is less potential for diagonal movement
    if (isShortJourney)
    {
        --dy_diag;
    }

    // First figure out how far we can travel diagonally
    int numTiles = std::min(dx, dy_diag);
    dx -= numTiles;
    dy -= numTiles / 2;

    // For short journeys, we cover more y-distance when travelling diagonally
    if (isShortJourney)
    {
        --dy;
    }

    // Whatever's left must be covered by straight lines
    if (dy > 0)
    {
        numTiles += dy;
    }
    if (dx > 0)
    {
        // We can cover 2 tiles with each horizontal movement.
        // Round up because we might need 1 final diagonal movement to change between upper/lower tiles.
        numTiles += static_cast<int>(std::ceil(static_cast<float>(dx) / eastWestTileSpan));
    }

    return numTiles;
}

}}  // namespace Rival::MapUtils
