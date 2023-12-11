#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace Rival {

class Entity;

// Use these when Entity references need to be kept alive
using SharedEntityList = std::vector<std::shared_ptr<const Entity>>;
using SharedMutableEntityList = std::vector<std::shared_ptr<Entity>>;

// Use these when Entity references are allowed to become invalid
using WeakEntityList = std::vector<std::weak_ptr<const Entity>>;
using WeakMutableEntityList = std::vector<std::weak_ptr<Entity>>;

/**
 * Interface exposing entities for read-only access.
 */
class EntityContainer
{
public:
    /**
     * Calls a function for each entity within the container (read-only version).
     */
    virtual void forEachMutableEntity(std::function<void(std::shared_ptr<Entity>)> func) = 0;
};

/**
 * Interface exposing entities for read-write access.
 */
class WritableEntityContainer : public EntityContainer
{
public:
    /**
     * Calls a function for each entity within the container (mutable version).
     */
    virtual void forEachEntity(std::function<void(std::shared_ptr<const Entity>)> func) const = 0;
};

}  // namespace Rival
