#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(UnitType type) :
        type(type),
        animation(type) {}

    void Unit::addedToWorld(
            int id, int player, int x, int y, Facing facing) {
        this->id = id;
        this->player = player;
        this->x = x;
        this->y = y;
        animation.setFacing( facing );
    }

    int Unit::getCurrentSpriteIndex() const {
        return animation.getCurrentSpriteIndex();
    }

    const UnitType Unit::getType() const {
        return type;
    }

    void Unit::setAnimation(UnitAnimationType unitAnimationType) {
        animation.setAnimation(unitAnimationType);
        std::cout << "Setting animation for unit " << id
                  << " of type " << static_cast<int>(type)
                  << ": " << static_cast<int>( unitAnimationType ) << '\n';
    }

    void Unit::rotateLeft() {
        animation.rotateLeft();
    }

    void Unit::rotateRight() {
        animation.rotateRight();
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

    void Unit::tick() {
        animation.tick();
    }
}
