#include "pch.h"
#include "UnitPropsComponent.h"

namespace Rival {

    const std::string UnitPropsComponent::key = "unit_props";

    UnitPropsComponent::UnitPropsComponent(Unit::Type type)
        : EntityComponent(key),
          type(type) {}

    void UnitPropsComponent::setState(UnitState newState) {
        state = newState;
    }

}  // namespace Rival
