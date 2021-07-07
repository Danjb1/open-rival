#include "pch.h"
#include "BuildingPropsComponent.h"

namespace Rival {

    const std::string BuildingPropsComponent::key = "building_props";

    BuildingPropsComponent::BuildingPropsComponent(
            Building::Type type)
        : EntityComponent(key),
          type(type) {}

}  // namespace Rival
