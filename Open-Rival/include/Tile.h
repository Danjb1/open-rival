#ifndef TILE_H
#define TILE_H

#include <cstdint>

namespace Rival {

    enum TileType: std::int8_t {
        Grass,
        Coastline,
        Water,
        Mud,
        Dirt,
        Dungeon,
        Gold,
        Cropland
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

}

#endif // TILE_H
