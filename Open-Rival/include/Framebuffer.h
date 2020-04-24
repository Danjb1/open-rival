#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <gl/glew.h>

namespace Rival {

    class Framebuffer {

    public:

        Framebuffer(GLuint id, GLuint textureId, int width, int height);

        GLuint getId() const;

        GLuint getTextureId() const;

        int getWidth() const;

        int getHeight() const;

        static const Framebuffer generate(int width, int height);

    private:

        GLuint id;

        GLuint textureId;

        int width;

        int height;

    };

}

#endif // FRAME_BUFFER_H
