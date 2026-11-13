#include "game/World.h"

#include "utils/IterationGuard.h"
#include "utils/LogUtils.h"

namespace Rival {

// Creates an empty World
World::World(int width, int height, bool wilderness)
    : spatialIndex(width, height)
    , wilderness(wilderness)

    // Default to Grass everywhere
    , tiles(std::vector<Tile>(width * height, Tile(TileType::Grass, 0, 0)))
    , passabilityMap(width, height, TilePassability::Clear)
    , nextId(0)
{
}

// Creates a World from data
World::World(int width, int height, bool wilderness, std::vector<Tile> tiles)
    : spatialIndex(width, height)
    , wilderness(wilderness)
    , tiles(tiles)
    , passabilityMap(width, height, createPassability(width, height))
    , nextId(0)
{
}

std::vector<TilePassability> World::createPassability(int width, int height) const
{
    std::vector<TilePassability> passability;
    passability.reserve(tiles.size());

    for (int i = 0; i < tiles.size(); ++i)
    {
        const auto& tile = tiles[i];
        int x = i % width;
        int y = i / width;

        if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
        {
            // Tiles at the edge of the map are impassable
            passability.push_back(TilePassability::Impassable);
        }
        else if (tile.type == TileType::Water)
        {
            passability.push_back(TilePassability::Water);
        }
        else if (tile.type == TileType::Coastline)
        {
            passability.push_back(TilePassability::Coastline);
        }
        else
        {
            passability.push_back(TilePassability::Clear);
        }
    }

    return passability;
}

void World::onAllEntitiesCreated()
{
    passabilityMap.initReachability();
}

Tile World::getTile(int x, int y) const
{
    return tiles[y * getWidth() + x];
}

void World::addEntity(std::shared_ptr<Entity> entity, int x, int y)
{
    addEntity(entity, { x, y });
}

void World::addEntity(std::shared_ptr<Entity> entity, const MapNode& pos)
{
    if (isIteratingEntities())
    {
        // Adding to the Entity list now would invalidate the iterator in use, so defer it instead
        LOG_ERROR("Tried to add an Entity while iterating over Entities; use requestAddEntity instead");
        requestAddEntity(entity, pos.x, pos.y);
        return;
    }

    entitiesList.push_back(entity);
    entitiesById[nextId] = entity;
    spatialIndex.addEntity({ nextId, pos, entity->isBig() });
    entity->addedToWorld(this, nextId, pos);
    ++nextId;
}

void World::requestAddEntity(std::shared_ptr<Entity> entity, int x, int y)
{
    pendingEntities.push_back({ entity, x, y });
}

void World::addPendingEntities()
{
    if (isIteratingEntities())
    {
        LOG_ERROR("Tried to add pending Entities while iterating over Entities");
        return;
    }

    for (const auto& pendingEntity : pendingEntities)
    {
        addEntity(pendingEntity.entity, pendingEntity.x, pendingEntity.y);
    }
    pendingEntities.clear();
}

void World::removeEntity(std::shared_ptr<Entity> entity)
{
    if (isIteratingEntities())
    {
        LOG_ERROR("Tried to remove an Entity while iterating over Entities; mark it for deletion instead");
        return;
    }

    const auto it = std::find(entitiesList.begin(), entitiesList.end(), entity);
    if (it != entitiesList.end())
    {
        entitiesList.erase(it);
    }

    entitiesById.erase(entity->getId());
    spatialIndex.removeEntity(entity->getId());
}

SharedMutableEntityList World::getMutableEntities() const
{
    return entitiesList;
}

SharedEntityList World::getEntities() const
{
    // Make a copy of the original vector with each element implicitly made const
    return { entitiesList.cbegin(), entitiesList.cend() };
}

SharedEntityList World::getEntitiesFromIds(const std::vector<int>& entityIds) const
{
    SharedEntityList entityList;

    for (int id : entityIds)
    {
        if (auto e = getEntityShared(id))
        {
            entityList.push_back(e);
        }
    }

    return entityList;
}

SharedMutableEntityList World::getMutableEntitiesFromIds(const std::vector<int>& entityIds) const
{
    SharedMutableEntityList entityList;

    for (int id : entityIds)
    {
        if (auto e = getMutableEntityShared(id))
        {
            entityList.push_back(e);
        }
    }

    return entityList;
}

void World::forEachMutableEntity(const std::function<void(std::shared_ptr<Entity>)>& func)
{
    IterationGuard guard(entityIterationDepth);
    for (const auto& entity : entitiesList)
    {
        func(entity);
    }
}

void World::forEachEntity(const std::function<void(std::shared_ptr<const Entity>)>& func) const
{
    IterationGuard guard(entityIterationDepth);
    for (const auto& entity : entitiesList)
    {
        func(entity);
    }
}

SharedEntityList World::getEntitiesInRadius(const MapNode& pos, int radius) const
{
    std::vector<int> entityIds = spatialIndex.getEntitiesInRadius(pos, radius);
    return getEntitiesFromIds(entityIds);
}

SharedMutableEntityList World::getMutableEntitiesInRadius(const MapNode& pos, int radius)
{
    std::vector<int> entityIds = spatialIndex.getEntitiesInRadius(pos, radius);
    return getMutableEntitiesFromIds(entityIds);
}

SharedEntityList World::getEntitiesAt(const MapNode& pos) const
{
    std::vector<int> entityIds = spatialIndex.getEntitiesAt(pos);
    return getEntitiesFromIds(entityIds);
}

SharedMutableEntityList World::getMutableEntitiesAt(const MapNode& pos)
{
    std::vector<int> entityIds = spatialIndex.getEntitiesAt(pos);
    return getMutableEntitiesFromIds(entityIds);
}

Entity* World::getMutableEntity(int id) const
{
    const auto iter = entitiesById.find(id);
    if (iter == entitiesById.cend())
    {
        return nullptr;
    }
    return (iter->second).get();
}

const Entity* World::getEntity(int id) const
{
    return getMutableEntity(id);
}

std::shared_ptr<Entity> World::getMutableEntityShared(int id) const
{
    const auto iter = entitiesById.find(id);
    return iter == entitiesById.cend() ? std::shared_ptr<Entity>() : iter->second;
}

std::shared_ptr<const Entity> World::getEntityShared(int id) const
{
    return getMutableEntityShared(id);
}

std::weak_ptr<Entity> World::getMutableEntityWeak(int id) const
{
    const auto iter = entitiesById.find(id);
    return iter == entitiesById.cend() ? std::weak_ptr<Entity>() : iter->second;
}

std::weak_ptr<const Entity> World::getEntityWeak(int id) const
{
    return getMutableEntityWeak(id);
}

void World::onEntityMoved(int entityId, const MapNode& newPos)
{
    spatialIndex.moveEntity(entityId, newPos);
}

int Rival::World::getWidth() const
{
    return passabilityMap.getWidth();
}

int Rival::World::getHeight() const
{
    return passabilityMap.getHeight();
}

}  // namespace Rival
