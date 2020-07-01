#include "pch.h"
#include "UnitAnimation.h"

#include <stdexcept>

namespace Rival {

    using TimerUtils::getCurrentTime;
    using TimerUtils::getTimeSince;

    UnitAnimation::UnitAnimation(UnitType unitType)
        : unitAnimationPair(std::make_pair(unitType, UnitAnimationType::Standing)),
          facing(Facing::South),
          animationTick(0) {
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

    void UnitAnimation::setAnimation(UnitAnimationType unitAnimationType) {
        unitAnimationPair.second = unitAnimationType;
        updateSpriteSheetEntry();
        animationTick = 0;
    }

    void UnitAnimation::setSpeedCoefficient(int numerator, int denominator) {
        speedCoefficientNumerator = numerator;
        speedCoefficientDenominator = denominator;
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
        // Get number of animation frames
        auto spritesheetSpan = std::get<1>(spritesheetEntry) - std::get<0>(spritesheetEntry) + 1;
        // Return if there is only one animation frame
        if (spritesheetSpan == 1) {
            return;
        }

        ++animationTick;
        //static_assert( 3/2 == 1, "3/2 != 1" ); // true
        // Using tick based animation steps introduce some problems when
        //     an animation period is not a multiple of our tick period.
        // In case we have such situation, our best bet is to rearrange
        //     speedCoefficient operation as below.
        if (std::get<2>(spritesheetEntry) * speedCoefficientDenominator <= animationTick * speedCoefficientNumerator) {
            animationStep = (animationStep + 1) % (spritesheetSpan);
            animationTick = 0;
        }
    }
}  // namespace Rival
