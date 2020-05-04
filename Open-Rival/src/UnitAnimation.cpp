#include "pch.h"
#include "UnitAnimation.h"

#include <stdexcept>

namespace Rival {

    using TimerUtils::getCurrentTime;
    using TimerUtils::getTimeSince;

    UnitAnimation::UnitAnimation(UnitType unitType) :
        unitAnimationPair(std::make_pair(unitType, UnitAnimationType::Standing)),
        facing(Facing::South),
        lastAnimationUpdate(getCurrentTime()) {
        updateSpriteSheetEntry();
    }

    void UnitAnimation::setFacing(Facing newFacing) {
        facing = newFacing;
    }

    void UnitAnimation::rotateLeft() {
        int newFacing = (static_cast<int>(facing) - 1) % 8;
        setFacing(static_cast<Facing>(newFacing));
    }

    void UnitAnimation::rotateRight() {
        int newFacing = (static_cast<int>(facing) + 1) % 8;
        setFacing(static_cast<Facing>(newFacing));
    }

    void UnitAnimation::setAnimation(UnitAnimationType unitAnimationType ) {
        unitAnimationPair.second = unitAnimationType;
        updateSpriteSheetEntry();
        lastAnimationUpdate = getCurrentTime();
    }

    int UnitAnimation::getCurrentSpriteIndex() const {
        auto spritesheetSpan = std::get<1>(spritesheetEntry) - std::get<0>(spritesheetEntry) + 1;
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
        auto spritesheetSpan = std::get<1>(spritesheetEntry) - std::get<0>(spritesheetEntry) + 1;
        if (1 == spritesheetSpan) {
            return;
        }

        while (getCurrentTime() > lastAnimationUpdate + std::get<2>(spritesheetEntry)) {
            animationStep = (animationStep + 1) % (spritesheetSpan);
            lastAnimationUpdate += std::get<2>(spritesheetEntry);
        }
    }
}
