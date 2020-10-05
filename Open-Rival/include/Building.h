#ifndef BUILDING_H
#define BUILDING_H

#include "BuildingAnimation.h"
#include "BuildingAnimationLookup.h"
#include "BuildingTypes.h"
#include "Entity.h"
#include "Spritesheet.h"

namespace Rival {

    class Building : public Entity {

    public:
        Building(BuildingType type);

        // Do not use! Use other `onSpawn` method instead.
        void onSpawn(int newId, int newX, int newY) override;

        void onSpawn(
                int newId,
                int newPlayer,
                int newX,
                int newY);

        int getCurrentSpriteIndex() const;

        const BuildingType getType() const;

    private:
        static const int numTilesX = 3;
        static const int numTilesY = 2;
        static const int numTilesWallX = 1;
        static const int numTilesWallY = 1;

        static int determineWidth(BuildingType type);
        static int determineHeight(BuildingType type);
        static bool isWall(BuildingType type);

        BuildingType type;

        int player;
    };

}  // namespace Rival

#endif  // BUILDING_H
