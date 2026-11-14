#pragma once

#include <cstddef>  // std::size_t
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace Rival {

/**
 * Interface exposing the map size.
 */
class MapBounds
{
public:
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

/**
 * The co-ordinates of a tile.
 */
struct MapNode
{
    static const MapNode Invalid;

    int x;
    int y;

    bool operator==(const MapNode& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const MapNode& other) const
    {
        return !(*this == other);
    }

    bool isValid() const
    {
        return x >= 0 && y >= 0;
    }

    std::string toString() const
    {
        return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
    }
};

/**
 * A compass direction.
 */
enum class Facing : std::uint8_t
{
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

/** Tiles spanned by a direct east/west movement. */
static constexpr int eastWestTileSpan = 2;

/** Maximum number of neighbors that 1 tile can have. */
static constexpr int maxNeighbors = 8;

/**
 * Determines if a tile is an "upper tile".
 *
 * Each row of tiles is a zigzag, and the upper tiles are those that are
 * higher up the screen within the row.
 */
inline bool isUpperTile(int tileX)
{
    return tileX % 2 == 0;
}

/**
 * Determines if a tile is a "lower tile".
 *
 * Each row of tiles is a zigzag, and the lower tiles are those that are
 * lower down the screen within the row.
 */
inline bool isLowerTile(int tileX)
{
    return !isUpperTile(tileX);
}

/**
 * Gets the vertical position of a tile, measured in half-rows.
 *
 * Because of the zigzag layout of the map, lower tiles (odd columns) sit half a row below upper tiles (even columns).
 * Measuring vertical position in half-rows accounts for this, so that every movement changes a tile's position by a
 * fixed amount:
 *
 *   - North / South: 2 half-rows
 *   - East / West:   2 columns
 *   - Diagonal:      1 column and 1 half-row
 */
int getHalfRow(const MapNode& node);

/**
 * Gets the tile in the given column at the given vertical position, measured in half-rows (see `getHalfRow`).
 *
 * `halfRow` must be even for upper tiles (even columns) and odd for lower tiles (odd columns).
 */
MapNode fromHalfRow(int x, int halfRow);

/**
 * The horizontal and vertical distance between 2 tiles, measured in columns and half-rows (see `getHalfRow`).
 *
 * Since every movement changes `columns + halfRows` by exactly 2, `columns` and `halfRows` are always either both
 * even or both odd. In other words, the difference between them is always even.
 */
struct TileOffset
{
    int columns = 0;
    int halfRows = 0;
};

/**
 * Gets the (absolute) horizontal and vertical distance between 2 tiles.
 */
TileOffset getTileOffset(const MapNode& a, const MapNode& b);

/**
 * Finds all valid neighbors of the given MapNode.
 */
std::vector<MapNode> findNeighbors(const MapNode& node, const MapBounds& area);

/**
 * Gets the neighbor in the given direction, if it exists.
 */
std::optional<MapNode> getNeighbor(const MapNode& node, Facing dir, const MapBounds& area);

/**
 * Determines if 2 tiles are neighbors.
 */
bool isNeighbor(const MapNode& a, const MapNode& b);

/**
 * Gets the direction that points most directly from one tile towards another, as seen on screen.
 *
 * For neighboring tiles, this is simply the direction of the movement between them. For example, if `to` is directly
 * above `from`, this will return `Facing::North`.
 *
 * If the MapNodes are identical, this returns South.
 */
Facing getDir(const MapNode& from, const MapNode& to);

/**
 * Gets the distance between 2 MapNodes, in tiles.
 *
 * For this method, all directions are considered equivalent (even though east-west travel actually represents a change
 * of 2 units in the x-axis).
 */
int getTileDistance(const MapNode& a, const MapNode& b);

/**
 * Gets the "logical distance" between 2 MapNodes.
 *
 * For this method, north-south travel and east-west travel are more expensive than diagonal movement. This is what the
 * original game uses for range and sight, as it gives a more "rounded" area than other methods.
 */
int getLogicalDistance(const MapNode& a, const MapNode& b);

/**
 * Returns the tile reached by making a number of movements in a straight line.
 *
 * The result may lie outside the map.
 */
MapNode moveInDirection(const MapNode& src, Facing dir, int numMoves);

}  // namespace MapUtils
}  // namespace Rival

namespace std {

/** Custom hash function for MapNode. */
template <>
struct hash<Rival::MapNode>
{
    inline std::size_t operator()(const Rival::MapNode& node) const noexcept
    {
        // Given that x/y will never exceed 16 bits, this ought to give a
        // totally unique value for each node.
        return node.x | (node.y << 16);
    }
};

/** Print method for MapNodes. */
inline std::ostream& operator<<(std::ostream& out, const Rival::MapNode& node)
{
    return out << node.x << ", " << node.y;
}

}  // namespace std
