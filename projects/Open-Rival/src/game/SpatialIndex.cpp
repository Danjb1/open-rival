#include "game/SpatialIndex.h"

#include <algorithm>  // std::max, std::min
#include <utility>    // std::as_const

#include "utils/LogUtils.h"

namespace Rival {

SpatialIndex::SpatialIndex(int width, int height)
    : width(width)
    , height(height)
    // Round up so that partial chunks at the edges of the map are included
    , numChunksX((width + chunkWidth - 1) / chunkWidth)
    , numChunksY((height + chunkHeight - 1) / chunkHeight)
    , chunks(static_cast<std::size_t>(numChunksX * numChunksY))
{
}

void SpatialIndex::addEntity(const SpatialEntity& entity)
{
    Chunk* chunk = getChunkAt(entity.pos);
    if (!chunk)
    {
        LOG_WARN("Tried to add out-of-bounds entity at ({}, {})", entity.pos.x, entity.pos.y);
        return;
    }

    entities.emplace(entity.id, entity);
    chunk->addEntity(entity.id);
    // TODO: Consider adding big entities to multiple chunks
}

void SpatialIndex::removeEntity(int entityId)
{
    auto iter = entities.find(entityId);
    if (iter == entities.cend())
    {
        // Entity is not present!
        return;
    }

    SpatialEntity& entity = iter->second;

    Chunk* chunk = getChunkAt(entity.pos);
    if (chunk)
    {
        chunk->removeEntity(entity.id);
    }
    else
    {
        LOG_WARN("Entity at ({}, {}) was not found in expected chunk", entity.pos.x, entity.pos.y);
    }

    entities.erase(entityId);
}

void SpatialIndex::moveEntity(int entityId, const MapNode& newPos)
{
    auto iter = entities.find(entityId);
    if (iter == entities.cend())
    {
        // Entity is not present!
        LOG_WARN("Tried to move Entity using invalid ID: {}", entityId);
        return;
    }

    SpatialEntity& entity = iter->second;
    Chunk* currentChunk = getChunkAt(entity.pos);
    Chunk* newChunk = getChunkAt(newPos);

    entity.pos = newPos;

    if (currentChunk == newChunk)
    {
        // Entity has not moved chunks
        return;
    }

    if (currentChunk)
    {
        currentChunk->removeEntity(entity.id);
    }
    else
    {
        LOG_WARN("Entity at ({}, {}) was not found in expected chunk", entity.pos.x, entity.pos.y);
    }

    if (!newChunk)
    {
        LOG_WARN("Tried to move entity out-of-bounds: ({}, {})", entity.pos.x, entity.pos.y);
        return;
    }

    newChunk->addEntity(entity.id);
}

std::vector<int> SpatialIndex::getEntitiesAt(const MapNode& pos) const
{
    const Chunk* chunk = getChunkAt(pos);
    if (!chunk)
    {
        return {};
    }

    std::vector<int> foundEntityIds;

    auto chunkEntityIds = chunk->getEntityIds();
    for (int entityId : chunkEntityIds)
    {
        auto iter = entities.find(entityId);
        if (iter == entities.cend())
        {
            // Entity not found!
            LOG_WARN("Chunk contains invalid entity ID: {}", entityId);
            continue;
        }

        const SpatialEntity& entity = iter->second;
        if (entity.pos == pos)
        {
            foundEntityIds.push_back(entityId);
        }
    }

    return foundEntityIds;
}

std::vector<int> SpatialIndex::getEntitiesInRadius(const MapNode& pos, int radius) const
{
    std::vector<const Chunk*> relevantChunks = getChunksInRadius(pos, radius);
    std::vector<int> foundEntityIds;

    for (const Chunk* chunk : relevantChunks)
    {
        auto chunkEntityIds = chunk->getEntityIds();
        for (int entityId : chunkEntityIds)
        {
            auto iter = entities.find(entityId);
            if (iter == entities.cend())
            {
                // Entity not found!
                LOG_WARN("Chunk contains invalid entity ID: {}", entityId);
                continue;
            }

            const SpatialEntity& entity = iter->second;
            const int distance = MapUtils::getLogicalDistance(entity.pos, pos);
            if (distance <= radius)
            {
                foundEntityIds.push_back(entityId);
            }
        }
    }

    return foundEntityIds;
}

SpatialIndex::Chunk* SpatialIndex::getChunk(int chunkX, int chunkY)
{
    // Reuse the const implementation
    return const_cast<Chunk*>(std::as_const(*this).getChunk(chunkX, chunkY));
}

const SpatialIndex::Chunk* SpatialIndex::getChunk(int chunkX, int chunkY) const
{
    if (chunkX < 0 || chunkX >= numChunksX || chunkY < 0 || chunkY >= numChunksY)
    {
        // Out of bounds
        return nullptr;
    }
    return &chunks[chunkY * numChunksX + chunkX];
}

SpatialIndex::Chunk* SpatialIndex::getChunkAt(const MapNode& pos)
{
    // Reuse the const implementation
    return const_cast<Chunk*>(std::as_const(*this).getChunkAt(pos));
}

const SpatialIndex::Chunk* SpatialIndex::getChunkAt(const MapNode& pos) const
{
    // Check the position itself, since integer division would round small negative values to chunk 0
    if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height)
    {
        return nullptr;
    }
    return getChunk(pos.x / chunkWidth, pos.y / chunkHeight);
}

std::vector<const SpatialIndex::Chunk*> SpatialIndex::getChunksInRadius(const MapNode& pos, int radius) const
{
    std::vector<const Chunk*> foundChunks;

    // Find the min and max points within the radius.
    // NOTE: For simplicity, we assume a rectangular radius here. This is NOT accurate, but it saves us finding every
    //   tile in the radius, which is a non-trivial problem. Worst case scenario, we return a few extra chunks than we
    //   need.
    MapNode min = MapUtils::moveInDirection(pos, Facing::NorthWest, radius);
    MapNode max = MapUtils::moveInDirection(pos, Facing::SouthEast, radius);

    // Clamp to the bounds of the map, since the radius may extend beyond it
    const int minChunkX = std::max(min.x / chunkWidth, 0);
    const int maxChunkX = std::min(max.x / chunkWidth, numChunksX - 1);
    const int minChunkY = std::max(min.y / chunkHeight, 0);
    const int maxChunkY = std::min(max.y / chunkHeight, numChunksY - 1);

    for (int chunkY = minChunkY; chunkY <= maxChunkY; ++chunkY)
    {
        for (int chunkX = minChunkX; chunkX <= maxChunkX; ++chunkX)
        {
            if (const Chunk* chunk = getChunk(chunkX, chunkY))
            {
                foundChunks.push_back(chunk);
            }
        }
    }

    return foundChunks;
}

void SpatialIndex::Chunk::addEntity(int entityId)
{
    entityIds.insert(entityId);
}

void SpatialIndex::Chunk::removeEntity(int entityId)
{
    entityIds.erase(entityId);
}

std::vector<int> SpatialIndex::Chunk::getEntityIds() const
{
    return std::vector<int>(entityIds.begin(), entityIds.end());
}

}  // namespace Rival
