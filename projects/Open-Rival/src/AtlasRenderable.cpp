#include "pch.h"

#include "AtlasRenderable.h"

#include "Shaders.h"

namespace Rival {

AtlasRenderable::AtlasRenderable(const TextureAtlas& texAtlas, int maxSprites)
    : texAtlas(texAtlas)
{

    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs
    glGenBuffers(1, &positionVbo);
    glGenBuffers(1, &texCoordVbo);
    glGenBuffers(1, &ibo);

    // Determine the primitive that should be drawn
    drawMode = (maxSprites == 1) ? GL_TRIANGLE_FAN : GL_TRIANGLES;

    // Determine the number of indices per sprite
    indicesPerSprite = (drawMode == GL_TRIANGLE_FAN) ? numIndicesForTriangleFan : numIndicesForTriangles;

    // Initialize position buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(
            Shaders::vertexAttribIndex,
            numVertexDimensions,
            GL_FLOAT,
            GL_FALSE,
            numVertexDimensions * sizeof(GLfloat),
            nullptr);
    int positionBufferSize = maxSprites * numVertexDimensions * indicesPerSprite * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, positionBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize tex co-ord buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    int texCoordBufferSize = maxSprites * numTexCoordDimensions * indicesPerSprite * sizeof(GLfloat);
    glVertexAttribPointer(
            Shaders::texCoordAttribIndex,
            numTexCoordDimensions,
            GL_FLOAT,
            GL_FALSE,
            numTexCoordDimensions * sizeof(GLfloat),
            nullptr);
    glBufferData(GL_ARRAY_BUFFER, texCoordBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize index buffer - this should never need to change
    std::vector<GLuint> indexData;
    indexData.reserve(maxSprites * indicesPerSprite);
    for (int i = 0; i < maxSprites; i++)
    {

        unsigned int startIndex = i * numVerticesPerSprite;

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
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, maxSprites * indicesPerSprite * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableVertexAttribArray(Shaders::vertexAttribIndex);
    glEnableVertexAttribArray(Shaders::texCoordAttribIndex);
}

}  // namespace Rival
