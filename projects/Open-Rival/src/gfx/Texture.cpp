#include "gfx/Texture.h"

#include <stdexcept>

#include "gfx/Image.h"

namespace Rival {

Texture::Texture(unsigned int id, int width, int height)
    : id(id)
    , width(width)
    , height(height)
{
}

unsigned int Texture::getId() const
{
    return id;
}

int Texture::getWidth() const
{
    return width;
}

int Texture::getHeight() const
{
    return height;
}

}  // namespace Rival
