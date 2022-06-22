#include "pch.h"

#include "Texture.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

namespace Rival {

Texture::Texture(const GLuint id, int width, int height)
    : id(id)
    , width(width)
    , height(height)
{
}

const GLuint Texture::getId() const
{
    return id;
}

const int Texture::getWidth() const
{
    return width;
}

const int Texture::getHeight() const
{
    return height;
}

const Texture Texture::loadTexture(const std::string filename)
{
    return Texture(0, 0, 0);
}

}  // namespace Rival
