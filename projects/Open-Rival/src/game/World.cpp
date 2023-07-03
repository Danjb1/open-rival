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
    // Add the Entity to the world
    entities[nextId] = entity;
    entities[nextId]->onSpawn(this, nextId, { x, y });

    // Increase the ID for the next one
    ++nextId;
}

void World::requestAddEntity(std::shared_ptr<Entity> entity, int x, int y)
{
    pendingEntities.push_back({ entity, x, y });
}

void World::addPendingEntities()
{
    for (auto const& pendingEntity : pendingEntities)
    {
        addEntity(pendingEntity.entity, pendingEntity.x, pendingEntity.y);
    }
    pendingEntities.clear();
}

void World::removeEntity(std::shared_ptr<Entity> entity)
{
    entities.erase(entity->getId());
}

const SharedMutableEntityList World::getMutableEntities() const
{
    SharedMutableEntityList entityList;
    entityList.reserve(entities.size());

    for (auto const& entry : entities)
    {
        entityList.push_back(entry.second);
    }

    return entityList;
}

const SharedEntityList World::getEntities() const
{
    SharedEntityList entityList;
    entityList.reserve(entities.size());

    for (auto const& entry : entities)
    {
        entityList.push_back(entry.second);
    }

    return entityList;
}

Entity* World::getMutableEntity(int id) const
{
    auto const iter = entities.find(id);
    if (iter == entities.cend())
    {
        return nullptr;
    }
    return (iter->second).get();
}

const Entity* World::getEntity(int id) const
{
    auto const iter = entities.find(id);
    if (iter == entities.cend())
    {
        return nullptr;
    }
    return (iter->second).get();
}

std::shared_ptr<Entity> World::getMutableEntityShared(int id) const
{
    auto const iter = entities.find(id);
    return iter == entities.cend() ? std::shared_ptr<Entity>() : iter->second;
}

std::shared_ptr<const Entity> World::getEntityShared(int id) const
{
    auto const iter = entities.find(id);
    return iter == entities.cend() ? std::shared_ptr<Entity>() : iter->second;
}

std::weak_ptr<Entity> World::getMutableEntityWeak(int id) const
{
    auto const iter = entities.find(id);
    return iter == entities.cend() ? std::weak_ptr<Entity>() : iter->second;
}

std::weak_ptr<const Entity> World::getEntityWeak(int id) const
{
    auto const iter = entities.find(id);
    return iter == entities.cend() ? std::weak_ptr<Entity>() : iter->second;
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
