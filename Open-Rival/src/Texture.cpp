#include "pch.h"
#include "Texture.h"

namespace Rival {

    Texture::Texture(const GLuint id, const int width, const int height) :
        id(id),
        width(width),
        height(height) {}

    GLuint Texture::getId() const {
        return id;
    }

    int Texture::getWidth() const {
        return width;
    }

    int Texture::getHeight() const {
        return height;
    }

}
