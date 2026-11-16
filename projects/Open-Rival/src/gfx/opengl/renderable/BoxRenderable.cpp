#include "gfx/opengl/renderable/BoxRenderable.h"

#include <algorithm>  // std::min

#include "gfx/opengl/GLUtils.h"
#include "gfx/opengl/Shaders.h"
#include "utils/LogUtils.h"

namespace Rival {

BoxRenderable::BoxRenderable(int maxBoxes)
    : maxBoxes(maxBoxes)
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
    glVertexAttribPointer(Shaders::vertexAttribIndex,
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
    glVertexAttribPointer(Shaders::colorAttribIndex,
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

void BoxRenderable::setBoxes(const std::vector<Box>& boxes)
{
    if (static_cast<int>(boxes.size()) > maxBoxes)
    {
        LOG_WARN("Tried to render {} boxes, but the maximum is {}", boxes.size(), maxBoxes);
    }
    numBoxes = std::min(static_cast<int>(boxes.size()), maxBoxes);

    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> colorData;
    vertexData.reserve(numBoxes * numVerticesPerBox * numVertexDimensions);
    colorData.reserve(numBoxes * numVerticesPerBox * numColorDimensions);

    for (int i = 0; i < numBoxes; ++i)
    {
        const Box& box = boxes[i];
        const float x1 = box.topLeft.x;
        const float y1 = box.topLeft.y;
        const float x2 = box.bottomRight.x;
        const float y2 = box.bottomRight.y;

        /* clang-format off */
        vertexData.insert(vertexData.end(), {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        });
        /* clang-format on */

        for (int vertex = 0; vertex < numVerticesPerBox; ++vertex)
        {
            colorData.insert(colorData.end(), { box.color.r, box.color.g, box.color.b, box.color.a });
        }
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(GLfloat), vertexData.data());

    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colorData.size() * sizeof(GLfloat), colorData.data());
}

void BoxRenderable::draw() const
{
    glBindVertexArray(vao);
    glDrawElements(drawMode, indicesPerBox * numBoxes, GL_UNSIGNED_INT, nullptr);
}

}  // namespace Rival
