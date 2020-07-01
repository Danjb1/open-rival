#include "pch.h"
#include "BuildingAnimation.h"

#include <stdexcept>

namespace Rival {

    BuildingAnimation::BuildingAnimation(BuildingType buildingType)
        : buildingAnimationPair(std::make_pair(buildingType, BuildingAnimationType::Constructing)) {
        updateSpriteSheetEntry();
    }

    void BuildingAnimation::setAnimation(BuildingAnimationType buildingAnimationType) {
        buildingAnimationPair.second = buildingAnimationType;
        updateSpriteSheetEntry();
    }

    int BuildingAnimation::getCurrentSpriteIndex() const {
        return spritesheetEntry.first + animationStep;
    }

    void BuildingAnimation::updateSpriteSheetEntry() {
        auto lookupIt = buildingAnimationLookup.find(buildingAnimationPair);
        if (buildingAnimationLookup.end() == lookupIt) {
            throw std::logic_error("Can't find SpritesheetEntry");
        }
        spritesheetEntry = lookupIt->second;
    }

    void BuildingAnimation::tick() {
        auto spritesheetSpan = spritesheetEntry.second - spritesheetEntry.first + 1;
        animationStep = (animationStep + 1) % (spritesheetSpan);
    }
}  // namespace Rival
