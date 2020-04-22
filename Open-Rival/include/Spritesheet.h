#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <gl/glew.h>
#include <iostream>
#include <vector>

#include "Texture.h"

namespace Rival {

    /**
     * Wrapper around a texture that defines how it divides into individual
     * images.
     */
    class Spritesheet {

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

        Spritesheet(const Texture&, int width, int height);

        const std::vector<GLfloat> getTexCoords(int index) const;

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

#endif // SPRITESHEET_H
