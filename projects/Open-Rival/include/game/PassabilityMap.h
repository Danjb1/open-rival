#pragma once

#include <unordered_set>
#include <vector>

#include "game/MapUtils.h"
#include "game/Tile.h"

namespace Rival {

using TileGroup = std::unordered_set<MapNode>;

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

    /** Determines whether one tile is reachable from another.
     * "Reachable" means a path can be found that is clear of buildings and obstacles.
     * This is only relevant for ground units, since flying units can go anywhere. */
    virtual bool isReachable(const MapNode& a, const MapNode& b) const = 0;
};

/**
 * Stores and exposes tile passabilities.
 */
class PassabilityMap : public PathfindingMap
{
public:
    /** Creates a PassabilityMap with all passability set to the given value. */
    PassabilityMap(int width, int height, TilePassability defaultTilePassability);

    /** Creates a PassabilityMap. */
    PassabilityMap(int width, int height, const std::vector<TilePassability>& tilePassability);

    virtual ~PassabilityMap() = default;

    /** This should be called when all tiles passabilities have been set,
     * i.e. after all Entities are added to the world. */
    void initReachability();

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    int getReachabilityGroupForTile(const MapNode& node) const;

    // Begin PathfindingMap override
    TilePassability getPassability(const MapNode& pos) const override;
    void setPassability(const MapNode& pos, TilePassability newPassability) override;
    void addPassability(const MapNode& pos, TilePassability newPassability) override;
    void removePassability(const MapNode& pos, TilePassability newPassability) override;
    bool isReachable(const MapNode& a, const MapNode& b) const override;
    // End PathfindingMap override

public:
    /** TilePassability values that block pathfinding (for ground units).
     * Flying units don't care about reachability since they can go anywhere. */
    static constexpr TilePassability unpathableFlags =  //
            TilePassability::Blocked                    //
            | TilePassability::Building                 //
            | TilePassability::SoftMountain             //
            | TilePassability::Tree                     //
            | TilePassability::Water                    //
            | TilePassability::Impassable;

private:
    int width;
    int height;
    std::vector<TilePassability> tilePassabilities;
    std::vector<TileGroup> reachabilityGroups;

    void refreshReachability(const MapNode& node);
    int mergeReachabilityGroups(const std::unordered_set<int>& foundGroupIds);
    bool areTilePassabilitiesEquivalent(TilePassability a, TilePassability b) const;
};

}  // namespace Rival
