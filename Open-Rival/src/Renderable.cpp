#include "pch.h"
#include "Renderable.h"

#include "Shaders.h"

namespace Rival {

    Renderable::Renderable() {

        // Create vertex array
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create position buffer
        glGenBuffers(1, &positionVbo);
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glVertexAttribPointer(
            textureShader.vertexAttribIndex,
            numVertexDimensions,
            GL_FLOAT,
            GL_FALSE,
            numVertexDimensions * sizeof(GLfloat),
            nullptr);

        // Create tex co-ord buffer
        glGenBuffers(1, &texCoordVbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        glVertexAttribPointer(
            textureShader.texCoordAttribIndex,
            numTexCoordDimensions,
            GL_FLOAT,
            GL_FALSE,
            numTexCoordDimensions * sizeof(GLfloat),
            nullptr);

        // Create index buffer
        glGenBuffers(1, &ibo);

        // Enable vertex attributes
        glEnableVertexAttribArray(textureShader.vertexAttribIndex);
        glEnableVertexAttribArray(textureShader.texCoordAttribIndex);
    }

    Renderable::~Renderable() {
        glDeleteBuffers(1, &positionVbo);
        glDeleteBuffers(1, &texCoordVbo);
        glDeleteBuffers(1, &ibo);
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

}
