#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(UnitType type)
        : deleted(false),
          id(-1),
          type(type) {}

    void Unit::addedToWorld(
            int newId, int newPlayer, int newX, int newY, Facing newFacing) {
        id = newId;
        player = newPlayer;
        x = newX;
        y = newY;
        facing = newFacing;
    }

    int Unit::getCurrentSpriteIndex() const {
        return 0;  //animation.getCurrentSpriteIndex();
    }

    const UnitType Unit::getType() const {
        return type;
    }

    void Unit::rotateLeft() {
        int newFacing = (static_cast<int>(facing) - 1) % numFacings;
        facing = static_cast<Facing>(newFacing);
    }

    void Unit::rotateRight() {
        int newFacing = (static_cast<int>(facing) + 1) % numFacings;
        facing = static_cast<Facing>(newFacing);
    }

    const bool Unit::isDeleted() const {
        return deleted;
    }

    void Unit::markForDeletion() {
        deleted = true;
    }

    const int Unit::getId() const {
        return id;
    }

    int Unit::getX() const {
        return x;
    }

    int Unit::getY() const {
        return y;
    }

    void Unit::update() {
        //animation.tick();
    }

}  // namespace Rival
