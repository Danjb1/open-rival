#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(const Sprite& sprite) :
        sprite(sprite) {}

    void Unit::addedToScene(const int newId) {
        id = newId;
    }

    const Sprite& Unit::getSprite() {
        return sprite;
    }

    std::uint8_t Unit::getFacing() const {
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

    const bool Unit::isDeleted() {
        return deleted;
    }

    void Unit::markForDeletion() {
        deleted = true;
    }

    const int Unit::getId() {
        return id;
    }

}
