#ifndef UNIT_H
#define UNIT_H

#include <vector>

#include "Entity.h"
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

    class Unit : public Entity {

    public:
        static const int numFacings = 8;

        Facing facing;

        Unit(UnitType type);

        // Do not use! Use other `onSpawn` method instead.
        void onSpawn(int newId, int newX, int newY) override;

        void onSpawn(
                int newId,
                int newPlayer,
                int newX,
                int newY,
                Facing newFacing);

        int getCurrentSpriteIndex() const;

        const UnitType getType() const;

        void rotateLeft();

        void rotateRight();

        void update();

    private:
        UnitType type;

        int player;
    };

}  // namespace Rival

#endif  // UNIT_H
