#ifndef SPRITE_H
#define SPRITE_H

#include <gl/glew.h>
#include <iostream>
#include <vector>

#include "Texture.h"

namespace Rival {

    /**
     * Class that defines how a texture should be divided into smaller images.
     */
    class Sprite {

    private:

        const Texture& texture;

        // Width of one image within the texture
        const int width;

        // Height of one image within the texture
        const int height;

        // Number of columns in the texture
        const int xSize;

        // Number of rows in the texture
        const int ySize;

    public:

        Sprite(const Texture&, const int width, const int height);

        std::vector<GLfloat> getTexCoords(const int index) const;

    };

}

#endif // SPRITE_H
