#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(UnitType type) :
        type(type) {}

    void Unit::addedToWorld(
            int id, int player, int x, int y, Facing facing) {
        this->id = id;
        this->player = player;
        this->x = x;
        this->y = y;
        this->facing = facing;
    }

    const UnitType Unit::getType() const {
        return type;
    }

    const Facing Unit::getFacing() const {
        return facing;
    }

    void Unit::rotateLeft() {
        int newFacing = (static_cast<int>(facing) - 1) % 8;
        setFacing(static_cast<Facing>(newFacing));
    }

    void Unit::rotateRight() {
        int newFacing = (static_cast<int>(facing) + 1) % 8;
        setFacing(static_cast<Facing>(newFacing));
    }

    void Unit::setFacing(Facing newFacing) {
        facing = newFacing;
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

}
