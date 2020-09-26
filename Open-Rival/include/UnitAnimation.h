#ifndef UNIT_ANIMATION_H
#define UNIT_ANIMATION_H

#include "TimerUtils.h"
#include "UnitAnimationLookup.h"

namespace Rival {

    class UnitAnimation {

    public:
        UnitAnimation(UnitType unitType);

        int getCurrentSpriteIndex() const;

        void setAnimation(UnitAnimationType unitAnimationType);

        void tick();

    private:
        void updateSpriteSheetEntry();

        int animationStep;

        int msPassedCurrentAnimFrame;

        UnitSpritesheetEntry spritesheetEntry;

        // Holds unitType and unitAnimationType
        UnitAnimationPair unitAnimationPair;
    };
}  // namespace Rival

#endif  // UNIT_ANIMATION_H
