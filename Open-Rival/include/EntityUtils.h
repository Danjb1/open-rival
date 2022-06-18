#pragma once

#include <memory>
#include <vector>

namespace Rival {

class Entity;

using EntityList = std::vector<std::shared_ptr<const Entity>>;
using MutableEntityList = std::vector<std::shared_ptr<Entity>>;

}  // namespace Rival
