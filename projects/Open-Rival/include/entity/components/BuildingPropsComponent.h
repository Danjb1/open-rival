#pragma once

#include <string>

#include "game/Building.h"
#include "entity/components/EntityComponent.h"

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
