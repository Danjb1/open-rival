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

        void updateSpriteSheetEntry();

        int animationStep;

        SpritesheetEntry spritesheetEntry;

        // Holds buildingType and buildingAnimationType
        BuildingAnimationPair buildingAnimationPair;

    };
}

#endif // BUILDING_ANIMATION_H
