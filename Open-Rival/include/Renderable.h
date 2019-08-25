#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Sprite.h"

namespace Rival {

    /**
     * Class containing all the data needed for OpenGL to render an image.
     */
    class Renderable {

    public:

        Renderable(const Sprite&);

        ~Renderable();

        // Disable moving / copying
        Renderable(const Renderable& other) = delete;
        Renderable(Renderable&& other) = delete;
        Renderable& operator=(const Renderable& other) = delete;
        Renderable& operator=(Renderable&& other) = delete;

        GLuint getPositionVbo() const;

        GLuint getTexCoordVbo() const;

        GLuint getIbo() const;

        std::vector<GLfloat> getTexCoords() const;

        GLuint getTextureId() const;

        int getTxIndex() const;

        void setTxIndex(int newTxIndex);

    private:

        const Sprite& sprite;

        GLuint positionVbo = 0;

        GLuint texCoordVbo = 0;

        GLuint ibo = 0;

        int txIndex = 0;

    };

}

#endif // RENDERABLE_H
