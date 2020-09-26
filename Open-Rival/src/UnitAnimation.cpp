#include "pch.h"
#include "UnitAnimation.h"

#include <stdexcept>

#include "Unit.h"

namespace Rival {

    UnitAnimation::UnitAnimation(UnitType unitType)
        : unitAnimationPair(std::make_pair(unitType, UnitAnimationType::Standing)),
          msPassedCurrentAnimFrame(0) {
        updateSpriteSheetEntry();
    }

    void UnitAnimation::setAnimation(UnitAnimationType unitAnimationType) {
        unitAnimationPair.second = unitAnimationType;
        updateSpriteSheetEntry();
        msPassedCurrentAnimFrame = 0;
    }

    int UnitAnimation::getCurrentSpriteIndex() const {
        auto spritesheetSpan = std::get<1>(spritesheetEntry) - std::get<0>(spritesheetEntry) + 1;
        // TODO: get current facing from Unit
        Facing facing = Facing::South;
        auto directionOffset = static_cast<int>(facing) - static_cast<int>(Facing::South);
        return std::get<0>(spritesheetEntry) + spritesheetSpan * directionOffset + animationStep;
    }

    void UnitAnimation::updateSpriteSheetEntry() {
        auto lookupIt = unitAnimationLookup.find(unitAnimationPair);
        if (unitAnimationLookup.end() == lookupIt) {
            throw std::logic_error("Can't find SpritesheetEntry");
        }
        spritesheetEntry = lookupIt->second;
    }

    void UnitAnimation::tick() {
        // Get number of animation frames
        auto spritesheetSpan = std::get<1>(spritesheetEntry) - std::get<0>(spritesheetEntry) + 1;

        // Return if there is only one animation frame
        if (spritesheetSpan == 1) {
            return;
        }

        ++msPassedCurrentAnimFrame;

        // TODO: This should vary based on the unit's speed
        int msPerAnimFrame = std::get<2>(spritesheetEntry);

        if (msPassedCurrentAnimFrame >= msPerAnimFrame) {
            animationStep = (animationStep + 1) % spritesheetSpan;
            msPassedCurrentAnimFrame -= msPerAnimFrame;
        }
    }

}  // namespace Rival
