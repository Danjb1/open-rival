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

    /** Determines whether one tile is reachable from another.
     * "Reachable" means a path can be found that is clear of buildings and obstacles.
     * This is only relevant for ground units, since flying units can go anywhere. */
    virtual bool isReachable(const MapNode& a, const MapNode& b) const = 0;
};

/**
 * Stores and manages the passability and reachability of all tiles.
 */
class PassabilityMap : public PathfindingMap
{
public:
    /** Creates a PassabilityMap with all passability set to the given value. */
    PassabilityMap(int width, int height, TilePassability defaultTilePassability);

    /** Creates a PassabilityMap. */
    PassabilityMap(int width, int height, const std::vector<TilePassability>& tilePassability);

    virtual ~PassabilityMap() = default;

    /** Calculates the reachability of every tile.
     * This must be called when all tiles passabilities have been set, i.e. after all Entities are added to the world,
     * and before reachability is queried. After this, reachability is kept up to date as tile passabilities change. */
    void initReachability();

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
    bool areTilePassabilitiesEquivalent(TilePassability a, TilePassability b) const;
    int getTileIndex(const MapNode& pos) const;
    bool isInBounds(const MapNode& pos) const;
    void updatePassability(const MapNode& pos, TilePassability newPassability);
    void removeTileFromGroup(const MapNode& pos);
    void addTileToGroup(const MapNode& pos);
    void splitGroupIfDisconnected(int group, std::vector<MapNode> nodes);
    void mergeGroups(int fromGroup, int intoGroup);
    void moveTileToGroup(int tileIndex, int newGroup);
    int createGroup();

private:
    /** Group ID used for tiles that do not belong to any group. */
    static constexpr int noGroup = -1;

    int width;
    int height;
    std::vector<TilePassability> tilePassabilities;

    /** Whether initReachability has been called. */
    bool isReachabilityInitialized = false;

    /*
     * Tiles are divided into reachability groups. Tiles in the same group are reachable from one another.
     *
     * We store both the group of each tile, and the tiles in each group. These must be kept in sync, so they should
     * only be modified via `moveTileToGroup`.
     */

    /** Group ID of each tile, indexed by tile index. */
    std::vector<int> tileGroups;

    /** Tile indices belonging to each group, indexed by group ID. */
    std::vector<std::unordered_set<int>> groups;

    /** IDs of groups that are empty and can be reused. */
    std::vector<int> emptyGroups;
};

}  // namespace Rival
