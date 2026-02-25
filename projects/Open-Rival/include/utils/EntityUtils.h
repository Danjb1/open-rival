#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "entity/Entity.h"

namespace Rival {

// Use these when Entity references need to be kept alive
using SharedEntityList = std::vector<std::shared_ptr<const Entity>>;
using SharedMutableEntityList = std::vector<std::shared_ptr<Entity>>;

// Use these when Entity references are allowed to become invalid
using WeakEntityList = std::vector<std::weak_ptr<const Entity>>;
using WeakMutableEntityList = std::vector<std::weak_ptr<Entity>>;

/**
 * Interface exposing a collection of entities.
 */
class EntityContainer
{
public:
    /** Calls a function for each entity within the container (read-only version). */
    virtual void forEachMutableEntity(const std::function<void(std::shared_ptr<Entity>)>& func) = 0;

    /** Calls a function for each entity within the container (mutable version). */
    virtual void forEachEntity(const std::function<void(std::shared_ptr<const Entity>)>& func) const = 0;

    /** Retrieves all entities within the given radius of a position (read-only version). */
    virtual SharedEntityList getEntitiesInRadius(const MapNode& pos, int radius) const = 0;

    /** Retrieves all entities within the given radius of a position (mutable version). */
    virtual SharedMutableEntityList getMutableEntitiesInRadius(const MapNode& pos, int radius) = 0;

    /** Gets the entities at the given position (read-only version). */
    virtual SharedEntityList getEntitiesAt(const MapNode& pos) const = 0;

    /** Gets the entities at the given position (mutable version). */
    virtual SharedMutableEntityList getMutableEntitiesAt(const MapNode& pos) = 0;
};

}  // namespace Rival
