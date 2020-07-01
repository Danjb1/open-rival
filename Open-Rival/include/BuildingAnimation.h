#ifndef BUILDING_ANIMATION_H
#define BUILDING_ANIMATION_H

#include "BuildingAnimationLookup.h"

namespace Rival {

    class BuildingAnimation {

    public:
        BuildingAnimation(BuildingType buildingType);

        int getCurrentSpriteIndex() const;

        void setAnimation(BuildingAnimationType buildingAnimationType);

        void tick();

    private:
        int animationStep;

        // Holds buildingType and buildingAnimationType
        BuildingAnimationPair buildingAnimationPair;

        SpritesheetEntry spritesheetEntry;

        void updateSpriteSheetEntry();
    };
}  // namespace Rival

#endif  // BUILDING_ANIMATION_H
