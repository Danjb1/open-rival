#pragma once

#include "entity/Entity.h"
#include "game/BuildingType.h"

namespace Rival {

/** A Building that can be added to the world. */
class Building : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Building;

public:
    Building(BuildingType type)
        : Entity(staticEntityType, /* isBig = */ !isWall(type))
        , type(type)
    {
    }

    BuildingType getBuildingType() const
    {
        return type;
    }

    bool isWall() const;

private:
    static bool isWall(BuildingType type);

private:
    BuildingType type;
};

}  // namespace Rival
