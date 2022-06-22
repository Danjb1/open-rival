#pragma once

#include <gl/glew.h>

#include <iostream>
#include <vector>

namespace Rival {

class Texture;

/**
 * Wrapper around a texture that defines how it divides into individual
 * images.
 */
class Spritesheet
{

public:
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

}  // namespace Rival
