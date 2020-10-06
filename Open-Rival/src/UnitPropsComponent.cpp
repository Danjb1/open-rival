#include "pch.h"
#include "UnitPropsComponent.h"

namespace Rival {

    const std::string UnitPropsComponent::key = "unit_props";

    UnitPropsComponent::UnitPropsComponent(Unit::Type type)
        : EntityComponent(key),
          type(type) {}

    Unit::Type UnitPropsComponent::getUnitType() const {
        return type;
    }

}  // namespace Rival
