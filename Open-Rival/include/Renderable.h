#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Sprite.h"

namespace Rival {

    /**
     * Class containing all the data needed for OpenGL to render an image.
     */
    class Renderable {

    private:

        const Sprite& sprite;

        GLuint positionVbo = 0;

        GLuint texCoordVbo = 0;

        GLuint ibo = 0;

    public:

        int txIndex = 0;

        Renderable(const Sprite&);

        ~Renderable();

        GLuint getPositionVbo() const;

        GLuint getTexCoordVbo() const;

        GLuint getIbo() const;

        std::vector<GLfloat> getTexCoords() const;

        GLuint getTextureId() const;

    };

}

#endif // RENDERABLE_H
