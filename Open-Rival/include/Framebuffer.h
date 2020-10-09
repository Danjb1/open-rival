#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <gl/glew.h>

namespace Rival {

    class Framebuffer {

    public:
        Framebuffer(int width, int height, bool useDepth);
        ~Framebuffer();

        // Prevent moving or copying (rule of 5)
        Framebuffer(const Framebuffer& other) = delete;
        Framebuffer(Framebuffer&& other) = delete;
        Framebuffer& operator=(const Framebuffer& other) = delete;
        Framebuffer& operator=(Framebuffer&& other) = delete;

        GLuint getId() const;

        GLuint getTextureId() const;

        int getWidth() const;

        int getHeight() const;

    private:
        GLuint id;

        GLuint textureId;
        GLuint depthRenderBufferId;

        int width;

        int height;

        void addDepthBuffer();
    };

}  // namespace Rival

#endif  // FRAME_BUFFER_H
