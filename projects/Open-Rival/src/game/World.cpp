#include "game/World.h"

namespace Rival {

// Creates an empty World
World::World(int width, int height, bool wilderness)
    : width(width)
    , height(height)
    , wilderness(wilderness)
    ,

    // Default to Grass everywhere
    tiles(std::vector<Tile>(width * height, Tile(TileType::Grass, 0, 0)))
    , tilePassability(std::vector<TilePassability>(width * height, TilePassability::Clear))
    , nextId(0)
{
}

// Creates a World from data
World::World(int width, int height, bool wilderness, std::vector<Tile> tiles)
    : width(width)
    , height(height)
    , wilderness(wilderness)
    , tiles(tiles)
    , tilePassability(createPassability())
    , nextId(0)
{
}

std::vector<TilePassability> World::createPassability() const
{
    std::vector<TilePassability> passability;
    passability.reserve(tiles.size());

    for (const auto& tile : tiles)
    {
        if (tile.type == TileType::Water)
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

Tile World::getTile(int x, int y) const
{
    return tiles[y * width + x];
}

void World::addEntity(std::shared_ptr<Entity> entity, int x, int y)
{
    entitiesList.push_back(entity);
    entitiesById[nextId] = entity;
    entity->addedToWorld(this, nextId, { x, y });
    ++nextId;
}

void World::requestAddEntity(std::shared_ptr<Entity> entity, int x, int y)
{
    pendingEntities.push_back({ entity, x, y });
}

void World::addPendingEntities()
{
    for (const auto& pendingEntity : pendingEntities)
    {
        addEntity(pendingEntity.entity, pendingEntity.x, pendingEntity.y);
    }
    pendingEntities.clear();
}

void World::removeEntity(std::shared_ptr<Entity> entity)
{
    const auto it = std::find(entitiesList.begin(), entitiesList.end(), entity);
    if (it != entitiesList.end())
    {
        entitiesList.erase(it);
    }

    entitiesById.erase(entity->getId());
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

void World::forEachMutableEntity(const std::function<void(std::shared_ptr<Entity>)>& func)
{
    for (const auto& entity : entitiesList)
    {
        func(entity);
    }
}

void World::forEachEntity(const std::function<void(std::shared_ptr<const Entity>)>& func) const
{
    for (const auto& entity : entitiesList)
    {
        func(entity);
    }
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
    const auto iter = entitiesById.find(id);
    if (iter == entitiesById.cend())
    {
        return nullptr;
    }
    return (iter->second).get();
}

const Entity* World::getEntityAt(const MapNode& pos) const
{
    // TODO: Add a more efficient way to retrieve entities by position
    // TODO: Add shared/mutable variants
    for (const auto& entity : entitiesList)
    {
        if (entity->getPos() == pos)
        {
            return entity.get();
        }
    }
    return nullptr;
}

std::shared_ptr<Entity> World::getMutableEntityShared(int id) const
{
    const auto iter = entitiesById.find(id);
    return iter == entitiesById.cend() ? std::shared_ptr<Entity>() : iter->second;
}

std::shared_ptr<const Entity> World::getEntityShared(int id) const
{
    const auto iter = entitiesById.find(id);
    return iter == entitiesById.cend() ? std::shared_ptr<Entity>() : iter->second;
}

std::weak_ptr<Entity> World::getMutableEntityWeak(int id) const
{
    const auto iter = entitiesById.find(id);
    return iter == entitiesById.cend() ? std::weak_ptr<Entity>() : iter->second;
}

std::weak_ptr<const Entity> World::getEntityWeak(int id) const
{
    const auto iter = entitiesById.find(id);
    return iter == entitiesById.cend() ? std::weak_ptr<Entity>() : iter->second;
}

int Rival::World::getWidth() const
{
    return width;
}

int Rival::World::getHeight() const
{
    return height;
}

TilePassability World::getPassability(const MapNode& pos) const
{
    return tilePassability[pos.y * width + pos.x];
}

void World::setPassability(const MapNode& pos, TilePassability passability)
{
    tilePassability[pos.y * width + pos.x] = passability;
}

}  // namespace Rival
