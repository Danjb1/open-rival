#include "pch.h"
#include "Framebuffer.h"

#include <iostream>
#include <stdexcept>

namespace Rival {

    Framebuffer::Framebuffer(
            const GLuint id, const GLuint textureId, int width, int height)
        : id(id),
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

    const Framebuffer Framebuffer::generate(
            int width, int height, bool useDepth) {

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

        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // Use linear filtering when downscaling so the game looks smooth when
        // the camera is zoomed out
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Set this as our colour attachement #0
        glFramebufferTexture(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);

        // Set the list of draw buffers
        GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);

        // Add the depth buffer, if required
        Framebuffer framebuffer = Framebuffer(id, textureId, width, height);
        if (useDepth) {
            framebuffer.addDepthBuffer();
        }

        // Check for errors
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer status: " << status << "\n";
            throw std::runtime_error("Failed to create framebuffer\n");
        }

        // Revert the state back to normal
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return framebuffer;
    }

    // See:
    // https://github.com/opengl-tutorials/ogl/blob/master/tutorial14_render_to_texture/tutorial14.cpp
    void Framebuffer::addDepthBuffer() {
        // Create a render buffer to store the depth information
        glGenRenderbuffers(1, &depthRenderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);
        glRenderbufferStorage(
                GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER, depthRenderBufferId);
    }

}  // namespace Rival
