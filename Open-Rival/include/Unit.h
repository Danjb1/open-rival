#ifndef UNIT_H
#define UNIT_H

#include "Sprite.h"

namespace Rival {

    class Unit {

    private:

        const Sprite& sprite;

        std::uint8_t facing;

    public:

        Unit(const Sprite&);

        const Sprite& getSprite() const;

        uint8_t getFacing() const;

        void setFacing(uint8_t facing);

    };

}

#endif // UNIT_H
