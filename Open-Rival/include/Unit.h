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

        Renderable& getRenderable();

        std::uint8_t getFacing() const;

        void setFacing(Facing facing);

        void rotateLeft();

        void rotateRight();

    private:

        Renderable renderable;

        Facing facing = Facing::south;

    };

}

#endif // UNIT_H
