#include "pch.h"
#include "Framebuffer.h"

#include <iostream>
#include <stdexcept>

namespace Rival {

    Framebuffer::Framebuffer(
            const GLuint id, const GLuint textureId, int width, int height) :
        id(id),
        textureId(textureId),
        width(width),
        height(height) {}

    GLuint Framebuffer::getId() const {
        return id;
    }

    GLuint Framebuffer::getTextureId() const {
        return textureId;
    }

    int Framebuffer::getWidth() const {
        return width;
    }

    int Framebuffer::getHeight() const {
        return height;
    }

    const Framebuffer Framebuffer::generate(int width, int height) {

        // Generate framebuffer
        GLuint id = 0;
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        // Create a texture to back this framebuffer
        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                width, height,
                0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Set this as our colour attachement #0
        glFramebufferTexture(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);

        // Set the list of draw buffers
        GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);

        // Check for errors
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer status:" << status << "\n";
            throw std::runtime_error("Failed to create framebuffer\n");
        }

        // Revert the state back to normal
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return Framebuffer(id, textureId, width, height);
    }

}
