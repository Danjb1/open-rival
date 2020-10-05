#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(UnitType type)
        : Entity(1, 1),
          type(type) {}

    void Unit::onSpawn(int newId, int newX, int newY) {
        onSpawn(newId, 0, newX, newY, Facing::South);
    }

    void Unit::onSpawn(
            int newId,
            int newPlayer,
            int newX,
            int newY,
            Facing newFacing) {
        player = newPlayer;
        facing = newFacing;
        Entity::onSpawn(newId, newX, newY);
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

}  // namespace Rival
