#include "game/PassabilityMap.h"

namespace Rival {

PassabilityMap::PassabilityMap(int width, int height, TilePassability defaultTilePassability)
    : width(width)
    , height(height)
    , tilePassability(width * height, defaultTilePassability)
{
}

PassabilityMap::PassabilityMap(int width, int height, const std::vector<TilePassability>& tilePassability)
    : width(width)
    , height(height)
    , tilePassability(tilePassability)
{
}

TilePassability PassabilityMap::getPassability(const MapNode& pos) const
{
    return tilePassability[pos.y * width + pos.x];
}

void PassabilityMap::setPassability(const MapNode& pos, TilePassability passability)
{
    tilePassability[pos.y * width + pos.x] = passability;
}

void PassabilityMap::addPassability(const MapNode& pos, TilePassability flags)
{
    tilePassability[pos.y * width + pos.x] |= flags;
}

void PassabilityMap::removePassability(const MapNode& pos, TilePassability flags)
{
    // Use bitwise AND with the inverted flags to remove the specified flags
    tilePassability[pos.y * width + pos.x] &= ~flags;
}

}  // namespace Rival
