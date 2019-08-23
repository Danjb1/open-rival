#include "pch.h"
#include "Renderable.h"

namespace Rival {

    Renderable::Renderable(const Sprite& sprite) :
            sprite(sprite) {

        glGenBuffers(1, &positionVbo);
        glGenBuffers(1, &ibo);
        glGenBuffers(1, &texCoordVbo);

        // Create IBO once, as this never changes
        // 4 vertices required to render a quad using GL_TRIANGLE_FAN
        const std::vector<GLuint> indexData = { 0, 1, 2, 3 };
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            4 * sizeof(GLuint),
            indexData.data(),
            GL_STATIC_DRAW);
    }

    Renderable::~Renderable() {
        std::cout << "destructor";
        glDeleteBuffers(1, &positionVbo);
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &texCoordVbo);
    }

    GLuint Renderable::getPositionVbo() const {
        return positionVbo;
    }

    GLuint Renderable::getTexCoordVbo() const {
        return texCoordVbo;
    }

    GLuint Renderable::getIbo() const {
        return ibo;
    }

    std::vector<GLfloat> Renderable::getTexCoords() const {
        return sprite.getTexCoords(txIndex);
    }

    GLuint Renderable::getTextureId() const {
        return sprite.texture.getId();
    }

    int Renderable::getTxIndex() const {
        return txIndex;
    }

    void Renderable::setTxIndex(int newTxIndex) {
        txIndex = newTxIndex;
    }

}
