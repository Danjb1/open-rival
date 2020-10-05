#include "pch.h"
#include "Building.h"

namespace Rival {

    Building::Building(BuildingType type)
        : Entity(determineWidth(type), determineHeight(type)),
          type(type) {}

    int Building::determineWidth(BuildingType type) {
        return isWall(type) ? numTilesWallX : numTilesX;
    }

    int Building::determineHeight(BuildingType type) {
        return isWall(type) ? numTilesWallY : numTilesY;
    }

    bool Building::isWall(BuildingType type) {
        return type == BuildingType::Wall
                || type == BuildingType::TreeWall
                || type == BuildingType::GreenskinWall;
    }

    void Building::onSpawn(int newId, int newX, int newY) {
        onSpawn(newId, 0, newX, newY);
    }

    void Building::onSpawn(
            int newId,
            int newPlayer,
            int newX,
            int newY) {
        id = newId;
        player = newPlayer;
        Entity::onSpawn(newId, newX, newY);
    }

    int Building::getCurrentSpriteIndex() const {
        return 0;
        /*
        auto spriteIndex = animation.getCurrentSpriteIndex();
        bool isAWall = (type == BuildingType::TreeWall)
                || (type == BuildingType::GreenskinWall)
                || (type == BuildingType::Wall);
        if (isAWall) {
            spriteIndex += static_cast<int>(wallVariant);
        }

        return spriteIndex;
        */
    }

    const BuildingType Building::getType() const {
        return type;
    }

}  // namespace Rival
