#include "pch.h"
#include "SpriteRenderable.h"

#include "Shaders.h"

namespace Rival {

    SpriteRenderable::SpriteRenderable(
            const Spritesheet& spritesheet,
            int maxSprites)
        : spritesheet(spritesheet) {

        // Generate VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generate VBOs
        glGenBuffers(1, &positionVbo);
        glGenBuffers(1, &texCoordVbo);
        glGenBuffers(1, &ibo);

        // Determine the primitive that should be drawn
        drawMode = (maxSprites == 1)
                ? GL_TRIANGLE_FAN
                : GL_TRIANGLES;

        // Determine the number of indices per sprite
        indicesPerSprite = (drawMode == GL_TRIANGLE_FAN)
                ? numIndicesForTriangleFan
                : numIndicesForTriangles;

        // Initialise position buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glVertexAttribPointer(
                indexedTextureShader.vertexAttribIndex,
                numVertexDimensions,
                GL_FLOAT,
                GL_FALSE,
                numVertexDimensions * sizeof(GLfloat),
                nullptr);
        int positionBufferSize = maxSprites
                * numVertexDimensions
                * indicesPerSprite
                * sizeof(GLfloat);
        glBufferData(
                GL_ARRAY_BUFFER,
                positionBufferSize,
                NULL,
                GL_DYNAMIC_DRAW);

        // Initialise tex co-ord buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        int texCoordBufferSize = maxSprites
                * numTexCoordDimensions
                * indicesPerSprite
                * sizeof(GLfloat);
        glVertexAttribPointer(
                indexedTextureShader.texCoordAttribIndex,
                numTexCoordDimensions,
                GL_FLOAT,
                GL_FALSE,
                numTexCoordDimensions * sizeof(GLfloat),
                nullptr);
        glBufferData(
                GL_ARRAY_BUFFER,
                texCoordBufferSize,
                NULL,
                GL_DYNAMIC_DRAW);

        // Initialise index buffer - this should never need to change
        std::vector<GLuint> indexData;
        indexData.reserve(maxSprites * indicesPerSprite);
        for (int i = 0; i < maxSprites; i++) {

            unsigned int startIndex = i * numVerticesPerSprite;

            if (drawMode == GL_TRIANGLE_FAN) {
                indexData.push_back(startIndex);
                indexData.push_back(startIndex + 1);
                indexData.push_back(startIndex + 2);
                indexData.push_back(startIndex + 3);

            } else if (drawMode == GL_TRIANGLES) {
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
                GL_ELEMENT_ARRAY_BUFFER,
                maxSprites * indicesPerSprite * sizeof(GLuint),
                indexData.data(),
                GL_STATIC_DRAW);

        // Enable vertex attributes
        glEnableVertexAttribArray(indexedTextureShader.vertexAttribIndex);
        glEnableVertexAttribArray(indexedTextureShader.texCoordAttribIndex);
    }

    GLuint SpriteRenderable::getVao() const {
        return vao;
    }

    GLuint SpriteRenderable::getPositionVbo() const {
        return positionVbo;
    }

    GLuint SpriteRenderable::getTexCoordVbo() const {
        return texCoordVbo;
    }

    GLuint SpriteRenderable::getIbo() const {
        return ibo;
    }

    GLuint SpriteRenderable::getTextureId() const {
        return spritesheet.texture.getId();
    }

    GLenum SpriteRenderable::getDrawMode() const {
        return drawMode;
    }

    int SpriteRenderable::getIndicesPerSprite() const {
        return indicesPerSprite;
    }

}  // namespace Rival
