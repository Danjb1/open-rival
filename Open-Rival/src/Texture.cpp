#include "pch.h"
#include "Texture.h"

#include <stdexcept>

#include "Image.h"

namespace Rival {

    Texture::Texture(const GLuint id, int width, int height) :
        id(id),
        width(width),
        height(height) {}

    //Texture::~Texture() {
    //    glDeleteTextures(1, &id);
    //}

    const GLuint Texture::getId() const {
        return id;
    }

    const int Texture::getWidth() const {
        return width;
    }

    const int Texture::getHeight() const {
        return height;
    }

    const Texture Texture::loadTexture(const std::string filename) {

        // Load image data
        Image img = Image::loadImage(filename);

        // Generate texture
        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            img.getWidth(), img.getHeight(),
            0, GL_RED, GL_UNSIGNED_BYTE, img.getData()->data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Check for error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("Error loading texture from %p pixels! %s\n",
                img.getData(), gluErrorString(error));
            throw std::runtime_error("Failed to load texture");
        }

        return Texture(textureId, img.getWidth(), img.getHeight());
    }

}
