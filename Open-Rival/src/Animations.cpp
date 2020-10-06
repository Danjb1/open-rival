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

}
}  // namespace Rival::Animations