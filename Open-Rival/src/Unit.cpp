#include "pch.h"
#include "Unit.h"

namespace Rival {

    Unit::Unit(UnitType type) :
            deleted(false),
            id(-1),
            type(type),
            animation(type) {
        setSpeedCoefficient(1, 1);
    }

    void Unit::addedToWorld(
            int newId, int newPlayer, int newX, int newY, Facing newFacing) {
        id = newId;
        player = newPlayer;
        x = newX;
        y = newY;
        animation.setFacing(newFacing);
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
                  << ": " << static_cast<int>(unitAnimationType) << '\n';
    }

    void Unit::setSpeedCoefficient(int numerator, int denominator) {
        speedCoefficientNumerator = numerator;
        speedCoefficientDenominator = denominator;
        animation.setSpeedCoefficient(numerator, denominator);
    }

    void Unit::setSpeedCoefficient(int numerator) {
        setSpeedCoefficient(numerator, 1);
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
