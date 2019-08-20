#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(const Sprite& sprite) :
        renderable(Renderable(sprite)) {}

    Renderable& Unit::getRenderable() {
        return renderable;
    }

    std::uint8_t Unit::getFacing() const {
        return facing;
    }

    void Unit::rotateLeft() {
        setFacing(facing - 1);
    }

    void Unit::rotateRight() {
        setFacing(facing + 1);
    }

    void Unit::setFacing(std::uint8_t newFacing) {
        facing = newFacing % 8;
    }

}
