#pragma once

#include "entity/Entity.h"
#include "game/BuildingType.h"

namespace Rival {

/** A Building that can be added to the world. */
class Building : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Building;
    static constexpr int defaultWidth = 3;
    static constexpr int defaultHeight = 2;
    static constexpr int wallWidth = 1;
    static constexpr int wallHeight = 1;

public:
    Building(BuildingType type)
        : Entity(staticEntityType, getWidth(type), getHeight(type))
        , type(type)
    {
    }

    BuildingType getBuildingType() const
    {
        return type;
    }

    bool isWall() const;

private:
    static int getWidth(BuildingType type);
    static int getHeight(BuildingType type);
    static bool isWall(BuildingType type);

private:
    BuildingType type;
};

}  // namespace Rival
