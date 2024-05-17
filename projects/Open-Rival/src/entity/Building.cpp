#include "entity/Building.h"

namespace Rival {

int Building::getWidth(BuildingType type)
{
    return isWall(type) ? wallWidth : defaultWidth;
}

int Building::getHeight(BuildingType type)
{
    return isWall(type) ? wallHeight : defaultHeight;
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
