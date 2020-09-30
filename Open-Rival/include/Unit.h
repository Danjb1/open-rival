#ifndef UNIT_H
#define UNIT_H

#include "Spritesheet.h"
#include "UnitAnimation.h"
#include "UnitAnimationLookup.h"
#include "UnitTypes.h"

namespace Rival {

    enum class Facing {
        South,
        SouthWest,
        West,
        NorthWest,
        North,
        NorthEast,
        East,
        SouthEast
    };

    class Unit {

    public:
        // Size of a Unit, in world units
        static const int width = 1;
        static const int height = 1;

        static const int numFacings = 8;

        Facing facing;

        Unit(UnitType type);

        void addedToWorld(int newId, int newPlayer, int newX, int newY, Facing newFacing);

        int getCurrentSpriteIndex() const;

        const UnitType getType() const;

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

        int player;

        /**
         * x co-ordinate of the Tile the Unit is occupying.
         *
         * It does not make sense to store the absolute position of a Unit
         * using floats, because the game world is not a continuous space; due
         * to the peculiar nature of the isometric tile grid, as x increases,
         * y fluctuates.
         *
         * Therefore, we simply store the tile co-ordinates of a Unit, and for
         * Units moving between tiles, we keep track of the movement direction
         * and a notion of progress towards the destination.
         */
        int x;

        /**
         * y co-ordinate of the Tile the Unit is occupying.
         */
        int y;
    };

}  // namespace Rival

#endif  // UNIT_H
