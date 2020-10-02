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

    // Tile passability, based on its contents.
    // This does not take into account flying units.
    enum class TilePassability {
        Clear,
        Unit,
        Building,
        Tree,
        SoftMountain,
        Blocked
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
