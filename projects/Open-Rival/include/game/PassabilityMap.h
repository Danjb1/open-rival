#pragma once

#include <unordered_set>
#include <vector>

#include "game/MapUtils.h"
#include "game/Tile.h"

namespace Rival {

/**
 * Interface exposing map data for pathfinding.
 */
class PathfindingMap : public MapBounds
{
public:
    /** Gets the passability of a tile. */
    virtual TilePassability getPassability(const MapNode& pos) const = 0;

    /** Sets the passability of a tile.
     * Use with caution; normally addPassability and removePassability are more appropriate. */
    virtual void setPassability(const MapNode& pos, TilePassability newPassability) = 0;

    /** Adds one or more passability flags to a tile. */
    virtual void addPassability(const MapNode& pos, TilePassability newPassability) = 0;

    /** Removes one or more passability flags from a tile. */
    virtual void removePassability(const MapNode& pos, TilePassability newPassability) = 0;
};

/**
 * Stores and manages the passability of all tiles.
 */
class PassabilityMap : public PathfindingMap
{
public:
    /** Creates a PassabilityMap with all passability set to the given value. */
    PassabilityMap(int width, int height, TilePassability defaultTilePassability);

    /** Creates a PassabilityMap. */
    PassabilityMap(int width, int height, const std::vector<TilePassability>& tilePassability);

    virtual ~PassabilityMap() = default;

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    // Begin PathfindingMap override
    TilePassability getPassability(const MapNode& pos) const override;
    void setPassability(const MapNode& pos, TilePassability newPassability) override;
    void addPassability(const MapNode& pos, TilePassability newPassability) override;
    void removePassability(const MapNode& pos, TilePassability newPassability) override;
    // End PathfindingMap override

private:
    int width;
    int height;
    std::vector<TilePassability> tilePassability;
};

}  // namespace Rival
