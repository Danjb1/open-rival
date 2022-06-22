#pragma once

#include "Building.h"
#include "EntityComponent.h"

namespace Rival {

/**
 * Component containing basic building properties.
 */
class BuildingPropsComponent : public EntityComponent
{

public:
    static const std::string key;

    BuildingPropsComponent(Building::Type type);

    Building::Type getBuildingType() const
    {
        return type;
    }

private:
    Building::Type type;
};

}  // namespace Rival
