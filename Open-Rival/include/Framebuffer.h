#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <gl/glew.h>

namespace Rival {

    class Framebuffer {

    public:

        Framebuffer(const GLuint id, int width, int height);

        //~Framebuffer();

        const GLuint id;

        const int width;

        const int height;

        static const Framebuffer generate(int width, int height);

    };

}

#endif // FRAME_BUFFER_H
