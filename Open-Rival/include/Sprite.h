#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"

namespace Rival {

    class Sprite {

    private:

        Texture& texture;

        // Width of one frame of the Sprite's texture
        int width;

        // Height of one frame of the Sprite's texture
        int height;

    public:

        Sprite(Texture& texture, int width, int height);

    };

}

#endif // SPRITE_H
