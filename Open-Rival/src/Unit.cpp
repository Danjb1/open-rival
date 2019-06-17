#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(const Sprite& sprite) :
        sprite(sprite) {}

    const Sprite& Unit::getSprite() const {
        return sprite;
    }

    uint8_t Unit::getFacing() const {
        return facing;
    }

    void Unit::setFacing(uint8_t facing) {
        this->facing = facing;
    }

}
