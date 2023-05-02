#include "pch.h"

#include "gfx/TextureRenderable.h"

#include <vector>

#include "GLUtils.h"
#include "Shaders.h"
#include "Texture.h"

namespace Rival {

TextureRenderable::TextureRenderable(std::shared_ptr<const Texture> texture)
    : texture(texture)
{
    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs
    glGenBuffers(1, &positionVbo);
    glGenBuffers(1, &texCoordVbo);
    glGenBuffers(1, &ibo);

    // Initialize position buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(
            Shaders::vertexAttribIndex,
            numVertexDimensions,
            GL_FLOAT,
            GL_FALSE,
            numVertexDimensions * sizeof(GLfloat),
            nullptr);
    int positionBufferSize = numVertexDimensions * GLUtils::numIndicesForTriangles * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, positionBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize tex co-ord buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    int texCoordBufferSize = numTexCoordDimensions * GLUtils::numIndicesForTriangles * sizeof(GLfloat);
    glVertexAttribPointer(
            Shaders::texCoordAttribIndex,
            numTexCoordDimensions,
            GL_FLOAT,
            GL_FALSE,
            numTexCoordDimensions * sizeof(GLfloat),
            nullptr);
    glBufferData(GL_ARRAY_BUFFER, texCoordBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize index buffer - this should never need to change
    std::vector<GLuint> indexData = { 0, 1, 2, 2, 3, 0 };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            GLUtils::numIndicesForTriangles * sizeof(GLuint),
            indexData.data(),
            GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableVertexAttribArray(Shaders::vertexAttribIndex);
    glEnableVertexAttribArray(Shaders::texCoordAttribIndex);
}

TextureRenderable::~TextureRenderable()
{
    if (!vao)
    {
        return;
    }

    // Clean up
    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glDeleteBuffers(1, &positionVbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        glDeleteBuffers(1, &texCoordVbo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glDeleteBuffers(1, &ibo);
    }
    glDeleteVertexArrays(1, &vao);
    glBindVertexArray(0);
}

GLuint TextureRenderable::getVao() const
{
    return vao;
}

GLuint TextureRenderable::getPositionVbo() const
{
    return positionVbo;
}

GLuint TextureRenderable::getTexCoordVbo() const
{
    return texCoordVbo;
}

GLuint TextureRenderable::getIbo() const
{
    return ibo;
}

GLuint TextureRenderable::getTextureId() const
{
    return texture->getId();
}

}  // namespace Rival
