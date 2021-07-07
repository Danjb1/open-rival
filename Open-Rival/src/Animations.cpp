#include "pch.h"
#include "Animations.h"

namespace Rival {
namespace Animations {

    Animation getUnitAnimation(
            Unit::Type unitType, UnitAnimationType animType) {
        return unitAnimationLookup.at({ unitType, animType });
    }

    Animation getBuildingAnimation(
            Building::Type buildingType, BuildingAnimationType animType) {
        return buildingAnimationLookup.at({ buildingType, animType });
    }

    Animation getObjectAnimation(std::uint8_t type, std::uint8_t variant) {
        return objectAnimationLookup.at({ type, variant });
    }

}}  // namespace Rival::Animations
