#include "pch.h"
#include "Renderable.h"

#include "Shaders.h"

namespace Rival {

    Renderable::Renderable(const Sprite& sprite) :
            sprite(sprite) {

        // Create vertex array
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create position buffer
        glGenBuffers(1, &positionVbo);
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glVertexAttribPointer(
            textureShader.vertexAttribIndex,
            2,
            GL_FLOAT,
            GL_FALSE,
            2 * sizeof(GLfloat),
            nullptr);

        // Create tex co-ord buffer
        glGenBuffers(1, &texCoordVbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        glVertexAttribPointer(
            textureShader.texCoordAttribIndex,
            2,
            GL_FLOAT,
            GL_FALSE,
            2 * sizeof(GLfloat),
            nullptr);

        // Create IBO once, as this never changes
        // 4 vertices required to render a quad using GL_TRIANGLE_FAN:
        //    0------1
        //    | \    |
        //    |   \..|
        //    3----- 2
        const std::vector<GLuint> indexData = { 0, 1, 2, 3 };

        // Create index buffer
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            4 * sizeof(GLuint),
            indexData.data(),
            GL_STATIC_DRAW);
    }

    Renderable::~Renderable() {
        glDeleteBuffers(1, &positionVbo);
        glDeleteBuffers(1, &positionVbo);
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &texCoordVbo);
        glDeleteVertexArrays(1, &vao);
    }

    GLuint Renderable::getVao() const {
        return vao;
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
