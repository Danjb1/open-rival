#include "pch.h"
#include "Texture.h"

#include <stdexcept>

#include "Image.h"

namespace Rival {

    Texture::Texture(const GLuint id, int width, int height) : id(id), width(width), height(height) {}

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
        TextureProperties props;  // use defaults
        return wrap(Image::readImage(filename), props);
    }

    const Texture Texture::wrap(const Image img, const TextureProperties props) {
        // Generate texture
        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,       // target slot
                GL_RED,  // 1 byte per pixel
                img.getWidth(),
                img.getHeight(),
                0,  // always zero!
                GL_RED,
                GL_UNSIGNED_BYTE,
                img.getData().data());

        // Set texture properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, props.magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, props.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, props.wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, props.wrapMode);

        // Revert the state back to normal
        glBindTexture(GL_TEXTURE_2D, 0);

        // Check for error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("Error loading texture: %s\n", gluErrorString(error));
            throw std::runtime_error("Failed to load texture");
        }

        return Texture(textureId, img.getWidth(), img.getHeight());
    }

}  // namespace Rival
