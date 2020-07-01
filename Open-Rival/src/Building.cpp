#include "pch.h"
#include "Building.h"

namespace Rival {

    Building::Building(BuildingType type)
        : deleted(false),
          id(-1),
          type(type),
          animation(type),
          wallVariant(WallVariant::A) {
    }

    void Building::addedToWorld(
            int newId, int newPlayer, int newX, int newY, WallVariant newWallVariant) {
        id = newId;
        player = newPlayer;
        x = newX;
        y = newY;
        wallVariant = newWallVariant;
    }

    int Building::getCurrentSpriteIndex() const {
        auto spriteIndex = animation.getCurrentSpriteIndex();
        bool isAWall = (type == BuildingType::TreeWall)
                || (type == BuildingType::GreenskinWall)
                || (type == BuildingType::Wall);
        if (isAWall) {
            spriteIndex += static_cast<int>(wallVariant);
        }

        return spriteIndex;
    }

    WallVariant Building::getWallVariant() const {
        return wallVariant;
    }

    const BuildingType Building::getType() const {
        return type;
    }

    void Building::setAnimation(BuildingAnimationType buildingAnimationType) {
        animation.setAnimation(buildingAnimationType);
        std::cout << "Setting animation for building " << id
                  << " of type " << static_cast<int>(type)
                  << ": " << static_cast<int>(buildingAnimationType) << '\n';
    }

    const bool Building::isDeleted() const {
        return deleted;
    }

    void Building::markForDeletion() {
        deleted = true;
    }

    const int Building::getId() const {
        return id;
    }

    int Building::getX() const {
        return x;
    }

    int Building::getY() const {
        return y;
    }

    void Building::tick() {
        animation.tick();
    }

}  // namespace Rival
