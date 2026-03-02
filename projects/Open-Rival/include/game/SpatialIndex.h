#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "game/MapUtils.h"

namespace Rival {

/** Simplified Entity representation used by SpatialIndex. */
class SpatialEntity
{
public:
    SpatialEntity(int id, const MapNode& pos, bool isBig)
        : id(id)
        , pos(pos)
        , isBig(isBig)
    {
    }

private:
    int id;
    MapNode pos;
    bool isBig;

    friend class SpatialIndex;
};

/** Class responsible for efficient indexing and retrieval of Entities using spatial partitioning.
 * This class uses its own internal Entity representation to keep it lightweight.
 * The SpatialIndex does not own the Entities; it just indexes them. To ensure correctness, the Entity owner must call
 * the appropriate methods whenever an Entity is added to / removed from the world, or moves between tiles. */
class SpatialIndex
{
public:
    /** Creates a SpatialIndex for the given world size. */
    SpatialIndex(int width, int height);

    /** Adds an Entity to the SpatialIndex. */
    void addEntity(const SpatialEntity& entity);

    /** Removes an Entity from the SpatialIndex. */
    void removeEntity(int entityId);

    /** Informs the SpatialIndex that an Entity has moved to a new tile.
     * This must be called for every movement, otherwise the spatial index will be incorrect! */
    void moveEntity(int entityId, const MapNode& newPos);

    /** Gets the IDs of all Entities occupying the given tile. */
    std::vector<int> getEntitiesAt(const MapNode& pos) const;

    /** Gets the IDs of all Entities within a radius of the given tile. */
    std::vector<int> getEntitiesInRadius(const MapNode& pos, int radius) const;

private:
    /** A section of the map. */
    class Chunk
    {
    public:
        void addEntity(int entityId);
        void removeEntity(int entityId);
        std::vector<int> getEntityIds() const;

    private:
        std::unordered_set<int> entityIds;
    };

    Chunk* getChunk(int chunkX, int chunkY);
    const Chunk* getChunk(int chunkX, int chunkY) const;
    Chunk* getChunkAt(const MapNode& pos);
    const Chunk* getChunkAt(const MapNode& pos) const;
    std::vector<const Chunk*> getChunksInRadius(const MapNode& pos, int radius) const;

private:
    static constexpr int chunkWidth = 8;
    static constexpr int chunkHeight = 8;

    int width;
    int height;
    std::vector<Chunk> chunks;
    std::unordered_map<int, SpatialEntity> entities;
};

}  // namespace Rival
