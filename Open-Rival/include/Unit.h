#ifndef UNIT_H
#define UNIT_H

#include "Renderable.h"
#include "Sprite.h"

namespace Rival {

    class Unit {

    public:

        const enum Facing {
            south,
            southWest,
            west,
            northWest,
            north,
            northEast,
            east,
            southEast
        };

        Unit(const Sprite& sprite);

        void addedToScene(const int newId);

        const Sprite& getSprite();

        std::uint8_t getFacing() const;

        void setFacing(Facing newFacing);

        void rotateLeft();

        void rotateRight();

        const bool isDeleted();

        void markForDeletion();

        const int getId();

    private:

        int id = -1;

        bool deleted = false;

        const Sprite& sprite;

        Facing facing = Facing::south;

    };

}

#endif // UNIT_H
