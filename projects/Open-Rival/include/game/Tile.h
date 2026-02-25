#pragma once

#include <cstdint>  // uint*_t

#include "utils/EnumUtils.h"

namespace Rival {

enum class TileType : std::uint8_t
{
    Grass,
    Coastline,
    Water,
    Mud,
    Dirt,
    Dungeon,
    Gold,
    Cropland
};

/**
 * Flags used to indicate tile passability.
 */
enum class TilePassability : std::uint16_t
{
    /**
     * Default value for empty (ground) tiles.
     */
    Clear = 0,

    /**
     * Flag set when a ground (or sea) unit occupies a tile.
     */
    GroundUnit = 1 << 1,

    /**
     * Flag set when a ground (or sea) unit is due to try and move out of a tile.
     */
    GroundUnitPendingMove = 1 << 2,

    /**
     * Flag set when a ground (or sea) unit is moving out of a tile.
     */
    GroundUnitLeaving = 1 << 3,

    /**
     * Flag set when a flying unit occupies a tile.
     */
    FlyingUnit = 1 << 4,

    /**
     * Flag set when a flying unit is due to try and move out of a tile.
     */
    FlyingUnitPendingMove = 1 << 5,

    /**
     * Flag set when a flying unit is moving into a tile.
     */
    FlyingUnitLeaving = 1 << 6,

    /**
     * Flag set when a building occupies a tile.
     */
    Building = 1 << 7,

    /**
     * Flag set when a tree occupies a tile.
     */
    Tree = 1 << 8,

    /**
     * Flag set when a soft mountain occupies a tile.
     */
    SoftMountain = 1 << 9,

    /**
     * Flag set when some other obstacle occupies a tile.
     */
    Blocked = 1 << 10,

    /**
     * Flag set when a tile contains water.
     */
    Water = 1 << 11,

    /**
     * Flag set when a tile contains a coastline.
     */
    Coastline = 1 << 12,

    /**
     * Flag set when a tile is impassable, even to flying units (e.g. the map edges).
     */
    Impassable = 1 << 13
};
MAKE_ENUM_FLAGS(TilePassability)

class Tile
{

public:
    Tile(const TileType type, const std::uint8_t txIndex, std::uint16_t resourceCount);

    TileType type;

    std::uint8_t txIndex;

private:
    std::uint16_t resourceCount;
};

}  // namespace Rival
