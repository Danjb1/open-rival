#include "pch.h"
#include "Framebuffer.h"

#include <stdexcept>

namespace Rival {

    Framebuffer::Framebuffer(const GLuint id, int width, int height) :
        id(id),
        width(width),
        height(height) {}

    const Framebuffer Framebuffer::generate(int width, int height) {

        GLuint id = 0;
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        // Create a texture to back this framebuffer
        GLuint renderedTexture;
        glGenTextures(1, &renderedTexture);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                width, height,
                0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Set this as our colour attachement #0
        glFramebufferTexture(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

        // Set the list of draw buffers
        GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);

        // Revert the state back to normal
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return Framebuffer(id, width, height);
    }

}
