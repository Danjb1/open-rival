#ifndef TILE_H
#define TILE_H

#include <cstdint>

namespace Rival {

    enum class TileType : std::uint8_t {
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
    enum class TilePassability : std::uint16_t {
        /**
         * Default value for empty (ground) tiles.
         */
        Clear = 0,

        /**
         * Flag set when a ground unit occupies a tile.
         */
        GroundUnit = 1 << 1,

        /**
         * Flag set when a flying unit occupies a tile.
         */
        FlyingUnit = 1 << 2,

        /**
         * Flag set when a building occupies a tile.
         */
        Building = 1 << 3,

        /**
         * Flag set when a tree occupies a tile.
         */
        Tree = 1 << 4,

        /**
         * Flag set when a soft mountain occupies a tile.
         */
        SoftMountain = 1 << 5,

        /**
         * Flag set when some other obstacle occupies a tile.
         */
        Blocked = 1 << 6,

        /**
         * Flag set when a tile contains water.
         */
        Water = 1 << 7,

        /**
         * Flag set when a tile contains a coastline.
         */
        Coastline = 1 << 8
    };

    class Tile {

    public:
        Tile(const TileType type,
                const std::uint8_t txIndex,
                std::uint16_t resourceCount);

        TileType type;

        std::uint8_t txIndex;

    private:
        std::uint16_t resourceCount;
    };

}  // namespace Rival

#endif  // TILE_H
