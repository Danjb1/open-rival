#include "gfx/renderable/BoxRenderable.h"

#include <vector>

#include "gfx/GLUtils.h"
#include "gfx/Shaders.h"

namespace Rival {

BoxRenderable::BoxRenderable(int maxBoxes)
{
    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs
    glGenBuffers(1, &positionVbo);
    glGenBuffers(1, &colorVbo);
    glGenBuffers(1, &ibo);

    // Determine the primitive that should be drawn
    drawMode = (maxBoxes == 1) ? GL_TRIANGLE_FAN : GL_TRIANGLES;

    // Determine the number of indices per box
    indicesPerBox = (drawMode == GL_TRIANGLE_FAN) ? GLUtils::numIndicesForTriangleFan : GLUtils::numIndicesForTriangles;

    // Initialize position buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(
            Shaders::vertexAttribIndex,
            numVertexDimensions,
            GL_FLOAT,
            GL_FALSE,
            numVertexDimensions * sizeof(GLfloat),
            nullptr);
    const int positionBufferSize = maxBoxes * numVertexDimensions * indicesPerBox * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, positionBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize color buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    const int colorBufferSize = maxBoxes * numColorDimensions * indicesPerBox * sizeof(GLfloat);
    glVertexAttribPointer(
            Shaders::colorAttribIndex,
            numColorDimensions,
            GL_FLOAT,
            GL_FALSE,
            numColorDimensions * sizeof(GLfloat),
            nullptr);
    glBufferData(GL_ARRAY_BUFFER, colorBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize index buffer - this should never need to change
    std::vector<GLuint> indexData;
    const int numIndices = maxBoxes * indicesPerBox;
    indexData.reserve(numIndices);
    for (int i = 0; i < maxBoxes; i++)
    {
        unsigned int startIndex = i * numVerticesPerBox;

        if (drawMode == GL_TRIANGLE_FAN)
        {
            indexData.push_back(startIndex);
            indexData.push_back(startIndex + 1);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 3);
        }
        else if (drawMode == GL_TRIANGLES)
        {
            indexData.push_back(startIndex);
            indexData.push_back(startIndex + 1);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 3);
            indexData.push_back(startIndex);
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableVertexAttribArray(Shaders::vertexAttribIndex);
    glEnableVertexAttribArray(Shaders::colorAttribIndex);
}

BoxRenderable::~BoxRenderable()
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
        glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
        glDeleteBuffers(1, &colorVbo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glDeleteBuffers(1, &ibo);
    }
    glDeleteVertexArrays(1, &vao);
    glBindVertexArray(0);
}

GLuint BoxRenderable::getVao() const
{
    return vao;
}

GLuint BoxRenderable::getPositionVbo() const
{
    return positionVbo;
}

GLuint BoxRenderable::getColorVbo() const
{
    return colorVbo;
}

GLuint BoxRenderable::getIbo() const
{
    return ibo;
}

GLenum BoxRenderable::getDrawMode() const
{
    return drawMode;
}

int BoxRenderable::getIndicesPerBox() const
{
    return indicesPerBox;
}

}  // namespace Rival
