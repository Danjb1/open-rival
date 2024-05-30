#pragma once

#include <cstddef>  // std::size_t
#include <optional>
#include <ostream>
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
    return tileX % 2 == 1;
}

/**
 * Finds all valid neighbors of the given MapNode.
 */
std::vector<MapNode> findNeighbors(const MapNode& node, const MapBounds& area);

/**
 * Gets the neighbour in the given direction, if it exists.
 */
std::optional<MapNode> getNeighbor(const MapNode& node, Facing dir, const MapBounds& area);

/**
 * Determines if 2 tiles are neighbors.
 */
bool isNeighbor(const MapNode& a, const MapNode& b);

/**
 * Gets the most pertinent direction between 2 neighbouring tiles.
 *
 * For example, if `to` is directly above `from`, this will return
 * `Facing::North`.
 *
 * If the MapNodes are identical, this returns South.
 */
Facing getDir(const MapNode& from, const MapNode& to);

/**
 * Gets the distance between 2 MapNodes.
 */
int getDistance(const MapNode& a, const MapNode& b);

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
