#include "entity/Building.h"

#include <magic_enum/magic_enum.hpp>

namespace Rival {

std::string Building::getDebugName() const
{
    return std::string(magic_enum::enum_name(type)) + " (" + std::to_string(id) + ")";
}

bool Building::isWall(BuildingType type)
{
    return type == BuildingType::Wall || type == BuildingType::TreeWall || type == BuildingType::GreenskinWall;
}

bool Building::isWall() const
{
    return isWall(type);
}

}  // namespace Rival
