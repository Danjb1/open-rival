#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "entity/Entity.h"
#include "game/MapUtils.h"
#include "game/Tile.h"
#include "utils/EntityUtils.h"

namespace Rival {

class Entity;

/**
 * Interface exposing map data for pathfinding.
 */
class PathfindingMap : public MapBounds
{
public:
    virtual TilePassability getPassability(const MapNode& pos) const = 0;
    virtual void setPassability(const MapNode& pos, TilePassability newPassability) = 0;
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
class World
    : public EntityContainer
    , public PathfindingMap
{
public:
    World(int width, int height, bool wilderness);
    World(int width, int height, bool wilderness, std::vector<Tile> tiles);
    virtual ~World() = default;

    // Begin EntityContainer override
    void forEachMutableEntity(const std::function<void(std::shared_ptr<Entity>)>& func) override;
    void forEachEntity(const std::function<void(std::shared_ptr<const Entity>)>& func) const override;
    // End EntityContainer override

    // Begin PathfindingMap override
    int getWidth() const override;
    int getHeight() const override;
    TilePassability getPassability(const MapNode& pos) const override;
    void setPassability(const MapNode& pos, TilePassability newPassability) override;
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
     * Gets a list of all entities currently present in the world (mutable version).
     *
     * Where possible, forEachMutableEntity should be used instead to avoid making a copy of this list.
     */
    SharedMutableEntityList getMutableEntities() const;

    /**
     * Gets a list of all entities currently present in the world (read-only version).
     *
     * Where possible, forEachEntity should be used instead to avoid making a copy of this list.
     */
    SharedEntityList getEntities() const;

    /**
     * Gets a raw pointer to the Entity with the given key (mutable version).
     *
     * This is not safe for long-term storage; the weak/shared pointer versions should be used instead.
     *
     * Returns nullptr if the Entity is not found.
     */
    Entity* getMutableEntity(int id) const;

    /**
     * Read-only version of `getMutableEntity`.
     */
    const Entity* getEntity(int id) const;

    /**
     * Gets the entity at the given tile position.
     */
    const Entity* getEntityAt(const MapNode& pos) const;

    /**
     * Gets a shared pointer to the Entity with the given key (mutable version).
     *
     * The weak pointer version should generally be preferred to avoid memory leaks, unless the Entity explicitly
     * needs to be kept alive even when it is removed from the level, e.g. for transported units or saving troops.
     *
     * Returns an empty shared_ptr if the Entity is not found.
     */
    std::shared_ptr<Entity> getMutableEntityShared(int id) const;

    /**
     * Read-only version of `getMutableEntityShared`.
     */
    std::shared_ptr<const Entity> getEntityShared(int id) const;

    /**
     * Gets a weak pointer to the Entity with the given key (mutable version).
     *
     * Returns an empty weak_ptr if the Entity is not found.
     */
    std::weak_ptr<Entity> getMutableEntityWeak(int id) const;

    /**
     * Read-only version of `getMutableEntityWeak`.
     */
    std::weak_ptr<const Entity> getEntityWeak(int id) const;

private:
    std::vector<TilePassability> createPassability() const;

private:
    const int width;
    const int height;
    bool wilderness;
    std::vector<Tile> tiles;
    std::vector<TilePassability> tilePassability;

    int nextId;
    std::vector<PendingEntity> pendingEntities;
    std::unordered_map<int, std::shared_ptr<Entity>> entitiesById;
    std::vector<std::shared_ptr<Entity>> entitiesList;
};

/**
 * Interface providing access to the world.
 */
class WorldStore
{
public:
    virtual World& getWorld() = 0;
};

}  // namespace Rival
