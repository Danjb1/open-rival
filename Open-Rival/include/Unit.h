#ifndef UNIT_H
#define UNIT_H

#include "Spritesheet.h"
#include "UnitAnimation.h"
#include "UnitAnimationLookup.h"
#include "UnitTypes.h"

namespace Rival {

    class Unit {

    public:
        // Size of a Unit, in world units
        static const int width = 1;
        static const int height = 1;

        Unit(UnitType type);

        void addedToWorld(int newId, int newPlayer, int newX, int newY, Facing newFacing);

        int getCurrentSpriteIndex() const;

        const UnitType getType() const;

        void setAnimation(UnitAnimationType unitAnimationType);

        void setSpeedCoefficient(int numerator, int denominator);

        void setSpeedCoefficient(int numerator);

        void rotateLeft();

        void rotateRight();

        const bool isDeleted() const;

        void markForDeletion();

        const int getId() const;

        int getX() const;

        int getY() const;

        void tick();

    private:
        int id;

        bool deleted;

        UnitType type;

        UnitAnimation animation;

        int speedCoefficientNumerator;

        int speedCoefficientDenominator;

        int player;

        /**
         * x co-ordinate of the Tile the Unit is occupying.
         */
        int x;

        /**
         * y co-ordinate of the Tile the Unit is occupying.
         */
        int y;
    };

}  // namespace Rival

#endif  // UNIT_H
