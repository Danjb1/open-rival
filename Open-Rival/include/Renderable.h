#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Sprite.h"

namespace Rival {

    /**
     * Class that wraps an OpenGL vertex array and associated buffers.
     */
    class Renderable {

    public:

        const int vertexDimensions = 2; // x, y
        const int texCoordDimensions = 2; // u, v

        Renderable();

        ~Renderable();

        // Disable moving / copying
        Renderable(const Renderable& other) = delete;
        Renderable(Renderable&& other) = delete;
        Renderable& operator=(const Renderable& other) = delete;
        Renderable& operator=(Renderable&& other) = delete;

        GLuint getVao() const;

        GLuint getPositionVbo() const;

        GLuint getTexCoordVbo() const;

        GLuint getIbo() const;

    protected:

        GLuint vao = 0;

        GLuint positionVbo = 0;

        GLuint texCoordVbo = 0;

        GLuint ibo = 0;

    };

}

#endif // RENDERABLE_H
