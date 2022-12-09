#pragma once

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

}  // namespace Rival
