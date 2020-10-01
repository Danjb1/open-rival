#ifndef BUILDING_H
#define BUILDING_H

#include "BuildingAnimation.h"
#include "BuildingAnimationLookup.h"
#include "BuildingTypes.h"
#include "Spritesheet.h"

namespace Rival {

    class Building {

    public:
        // Size of a Building, in tiles
        static const int width = 3;
        static const int height = 2;

        Building(BuildingType type);

        void addedToWorld(int newId,
                int newPlayer,
                int newX,
                int newY,
                WallVariant newWallVariant);

        int getCurrentSpriteIndex() const;

        WallVariant getWallVariant() const;

        const BuildingType getType() const;

        void setAnimation(BuildingAnimationType buildingAnimationType);

        const bool isDeleted() const;

        void markForDeletion();

        const int getId() const;

        int getX() const;

        int getY() const;

        void update();

    private:
        int id;

        bool deleted;

        BuildingType type;

        WallVariant wallVariant;

        BuildingAnimation animation;

        int player;

        /**
         * x co-ordinate of the bottom corner of the Building
         */
        int x;

        /**
         * y co-ordinate of the bottom corner of the Building
         */
        int y;
    };

}  // namespace Rival

#endif  // BUILDING_H
