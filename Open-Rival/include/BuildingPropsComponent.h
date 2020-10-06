#ifndef BUILDING_PROPS_COMPONENT_H
#define BUILDING_PROPS_COMPONENT_H

#include "Building.h"
#include "EntityComponent.h"

namespace Rival {

    class BuildingPropsComponent : public EntityComponent {

    public:
        static const std::string key;

        BuildingPropsComponent(Building::Type type);

        Building::Type getBuildingType() const;

    private:
        Building::Type type;
    };

}  // namespace Rival

#endif  // BUILDING_PROPS_COMPONENT_H
