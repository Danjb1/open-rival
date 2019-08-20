#ifndef UNIT_H
#define UNIT_H

#include "Renderable.h"
#include "Sprite.h"

namespace Rival {

    class Unit {

    private:

        Renderable renderable;

        std::uint8_t facing;

    public:

        Unit(const Sprite& sprite);

        Renderable& getRenderable();

        std::uint8_t getFacing() const;

        void setFacing(std::uint8_t newFacing);

        void rotateLeft();

        void rotateRight();

    };

}

#endif // UNIT_H
