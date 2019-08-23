#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(const Sprite& sprite) :
        renderable(sprite) {}

    Renderable& Unit::getRenderable() {
        return renderable;
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

}
