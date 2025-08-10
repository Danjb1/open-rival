#pragma once

#include "gfx/GLWrapper.h"

namespace Rival {

class Framebuffer
{

public:
    Framebuffer(int width, int height, bool useDepth);
    ~Framebuffer();

    // Prevent moving or copying (rule of 5)
    Framebuffer(const Framebuffer& other) = delete;
    Framebuffer(Framebuffer&& other) = delete;
    Framebuffer& operator=(const Framebuffer& other) = delete;
    Framebuffer& operator=(Framebuffer&& other) = delete;

    GLuint getId() const
    {
        return id;
    }

    GLuint getTextureId() const
    {
        return textureId;
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

private:
    GLuint id;

    GLuint textureId;
    GLuint depthRenderBufferId;

    int width;

    int height;

    void addDepthBuffer();
};

}  // namespace Rival
