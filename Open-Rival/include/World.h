#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "MapUtils.h"
#include "Tile.h"

namespace Rival {

class Entity;

/**
 * Interface exposing map data for pathfinding.
 */
class PathfindingMap : public MapBounds
{
public:
    virtual TilePassability getPassability(const MapNode& pos) const = 0;
};

/**
 * An Entity that is waiting to be added to the map.
 */
struct PendingEntity
{
    std::shared_ptr<Entity> entity;
    int x = 0;
    int y = 0;
};

/**
 * The game world.
 *
 * Contains map data and entities.
 */
class World : public PathfindingMap
{

public:
    World(int width, int height, bool wilderness);
    World(int width, int height, bool wilderness, std::vector<Tile> tiles);

    // Begin PathfindingMap override

    int getWidth() const override
    {
        return width;
    }

    int getHeight() const override
    {
        return height;
    }

    TilePassability getPassability(const MapNode& pos) const override;

    // End PathfindingMap override

    const std::vector<Tile>& getTiles() const
    {
        return tiles;
    }

    Tile getTile(int x, int y) const;

    bool isWilderness() const
    {
        return wilderness;
    }

    /**
     * Adds an Entity to the world immediately.
     *
     * Should not be called while iterating over Entities.
     */
    void addEntity(std::shared_ptr<Entity> entity, int x, int y);

    /**
     * Schedules an Entity to be added to the world.
     *
     * The Entity will be added the next time `addPendingEntities` is called.
     */
    void requestAddEntity(std::shared_ptr<Entity> entity, int x, int y);

    /**
     * Adds all pending Entities to the world immediately.
     *
     * Should not be called while iterating over Entities.
     */
    void addPendingEntities();

    /**
     * Removes an Entity from the world immediately.
     *
     * Should not be called while iterating over Entities. Mark Entities for deletion instead.
     */
    void removeEntity(std::shared_ptr<Entity> entity);

    /**
     * Returns a read-only list of all entities currently present in the world.
     */
    const std::vector<std::shared_ptr<Entity>> getEntities() const;

    /**
     * Gets an entity by its ID.
     *
     * We use shared_ptrs here because we sometimes need to maintain a
     * reference to an entity even when it is removed from the level, e.g.
     * for transported units or saving troops.
     *
     * If the entity is not found, an empty shared_ptr is returned.
     */
    const std::shared_ptr<Entity> getEntity(int id) const;

    void setPassability(const MapNode& pos, TilePassability passability);

private:
    const int width;
    const int height;
    bool wilderness;
    std::vector<Tile> tiles;
    std::vector<TilePassability> tilePassability;

    int nextId;
    std::vector<PendingEntity> pendingEntities;
    std::unordered_map<int, std::shared_ptr<Entity>> entities;
};

}  // namespace Rival
