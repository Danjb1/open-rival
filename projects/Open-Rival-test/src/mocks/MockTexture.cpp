/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include <memory>

#include "gfx/Texture.h"

namespace Rival {

Texture::Texture(const GLuint id, int width, int height)
    : id(id)
    , width(width)
    , height(height)
{
}

Texture ::~Texture() {}

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

std::shared_ptr<const Texture> Texture::loadTexture(const std::string& /*filename*/)
{
    return std::make_shared<const Texture>(0, 0, 0);
}

}  // namespace Rival
