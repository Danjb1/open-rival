#pragma once

#include "gfx/GLWrapper.h"

#include <memory>
#include <vector>

namespace Rival {

class Texture;

/**
 * Wrapper around a texture that defines how it divides into individual images.
 */
class Spritesheet
{
public:
    Spritesheet(std::shared_ptr<const Texture> texture, int width, int height, int padding);

    const std::vector<GLfloat> getTexCoords(int index) const;

public:
    std::shared_ptr<const Texture> texture;

private:
    // Width of one image within the texture
    const int width;

    // Height of one image within the texture
    const int height;

    // Padding around each image within the texture
    const int padding;

    // Width of one image - plus its padding - within the texture
    const int paddedWidth;

    // Height of one image - plus its padding - within the texture
    const int paddedHeight;

    // Number of columns in the texture
    const int xSize;

    // Number of rows in the texture
    const int ySize;
};

}  // namespace Rival
