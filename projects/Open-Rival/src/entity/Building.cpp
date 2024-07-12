#include "entity/Building.h"

namespace Rival {

bool Building::isWall(BuildingType type)
{
    return type == BuildingType::Wall || type == BuildingType::TreeWall || type == BuildingType::GreenskinWall;
}

bool Building::isWall() const
{
    return isWall(type);
}

}  // namespace Rival
