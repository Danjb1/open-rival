#ifndef IMAGE_RENDERABLE_H
#define IMAGE_RENDERABLE_H

#include "Renderable.h"
#include "Sprite.h"

namespace Rival {

    /**
     * Class containing all the data needed for OpenGL to render an image.
     */
    class ImageRenderable : public Renderable {

    public:

        const int indicesPerImage = 4;

        ImageRenderable(const Sprite&);

        // Disable moving / copying
        ImageRenderable(const ImageRenderable& other) = delete;
        ImageRenderable(ImageRenderable&& other) = delete;
        ImageRenderable& operator=(const ImageRenderable& other) = delete;
        ImageRenderable& operator=(ImageRenderable&& other) = delete;

        std::vector<GLfloat> getTexCoords() const;

        GLuint getTextureId() const;

        int getTxIndex() const;

        void setTxIndex(int newTxIndex);

    private:

        const Sprite& sprite;

        int txIndex = 0;

    };

}

#endif // IMAGE_RENDERABLE_H
