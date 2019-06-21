#ifndef UNIT_H
#define UNIT_H

#include "Renderable.h"
#include "Sprite.h"

namespace Rival {

    class Unit {

    private:

    public:

        const Renderable renderable;

        std::uint8_t facing;

        Unit(const Sprite& sprite);

    };

}

#endif // UNIT_H
