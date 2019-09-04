#include "pch.h"
#include "ImageRenderable.h"

#include "Shaders.h"

namespace Rival {

    ImageRenderable::ImageRenderable(const Sprite& sprite) :
            sprite(sprite) {

        // Create IBO once, as this never changes
        // 4 vertices required to render a quad using GL_TRIANGLE_FAN:
        //    0------1
        //    | \    |
        //    |   \..|
        //    3----- 2
        const std::vector<GLuint> indexData = { 0, 1, 2, 3 };
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indicesPerImage * sizeof(GLuint),
            indexData.data(),
            GL_STATIC_DRAW);
    }

    std::vector<GLfloat> ImageRenderable::getTexCoords() const {
        return sprite.getTexCoords(txIndex);
    }

    GLuint ImageRenderable::getTextureId() const {
        return sprite.texture.getId();
    }

    int ImageRenderable::getTxIndex() const {
        return txIndex;
    }

    void ImageRenderable::setTxIndex(int newTxIndex) {
        txIndex = newTxIndex;
    }

}
