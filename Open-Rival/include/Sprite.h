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

    public:

        // Size of a single Tile image, in pixels
        static const int tileSpriteWidthPx = 64;
        static const int tileSpriteHeightPx = 64;
        static const int tileWidthPx = 64;
        static const int tileHeightPx = 32;

        // Size of a single Unit image, in pixels
        static const int unitWidthPx = 128;
        static const int unitHeightPx = 128;

        const Texture& texture;

        Sprite(const Texture&, const int width, const int height);

        const std::vector<GLfloat> getTexCoords(const int index) const;

    private:

        // Width of one image within the texture
        const int width;

        // Height of one image within the texture
        const int height;

        // Number of columns in the texture
        const int xSize;

        // Number of rows in the texture
        const int ySize;

    };

}

#endif // SPRITE_H
