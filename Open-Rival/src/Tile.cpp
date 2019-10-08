#include "pch.h"
#include "Tile.h"

namespace Rival {

    Tile::Tile(const TileType type,
            const std::uint8_t txIndex,
            std::uint16_t resourceCount) :
        type(type),
        txIndex(txIndex),
        resourceCount(resourceCount) {}

}
