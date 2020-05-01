#ifndef UNIT_H
#define UNIT_H

#include "Spritesheet.h"
#include "UnitAnimation.h"
#include "UnitAnimationLookup.h"

namespace Rival {

    class Unit {

    public:

        // Size of a Unit, in world units
        static const int width = 1;
        static const int height = 1;

        Unit(UnitType type);

        void addedToWorld(int id, int player, int x, int y, Facing facing);

        int getCurrentSpriteIndex() const;

        const UnitType getType() const;

        void setAnimation( UnitAnimationType unitAnimationType );

        void rotateLeft();

        void rotateRight();

        const bool isDeleted() const;

        void markForDeletion();

        const int getId() const;

        int getX() const;

        int getY() const;

        void tick();

    private:

        int id = -1;

        bool deleted = false;

        UnitType type;

        UnitAnimation animation;

        int player;

        /// \todo getPos(), with std::atomic<std::pair<int, int>> pos instead of int x, int y would prevent race conditions
        /**
         * x co-ordinate of the Tile the Unit is occupying.
         */
        int x;

        /**
         * y co-ordinate of the Tile the Unit is occupying.
         */
        int y;

    };

}

#endif // UNIT_H
