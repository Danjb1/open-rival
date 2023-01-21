#include "pch.h"

#include "Texture.h"

#include <memory>

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

const std::shared_ptr<const Texture> Texture::loadTexture(const std::string filename)
{
    return std::make_shared<const Texture>(0, 0, 0);
}

}  // namespace Rival
