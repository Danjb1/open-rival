#include "pch.h"
#include "Texture.h"

namespace Rival {

    Texture::Texture(const GLuint id, const int width, const int height) :
        id(id),
        width(width),
        height(height) {}

    const GLuint Texture::getId() const {
        return id;
    }

    const int Texture::getWidth() const {
        return width;
    }

    const int Texture::getHeight() const {
        return height;
    }

}
