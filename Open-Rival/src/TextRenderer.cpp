#include "pch.h"
#include "TextRenderer.h"

#include "Framebuffer.h"
#include "Shaders.h"

namespace Rival {

    TextRenderer::TextRenderer() {

        // Generate VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generate VBOs
        glGenBuffers(1, &positionVbo);
        glGenBuffers(1, &texCoordVbo);
        glGenBuffers(1, &ibo);

        // Determine the number of indices per sprite
        indicesPerSprite = 4;

        // Initialize position buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glVertexAttribPointer(
                Shaders::vertexAttribIndex,
                numVertexDimensions,
                GL_FLOAT,
                GL_FALSE,
                numVertexDimensions * sizeof(GLfloat),
                nullptr);
        int positionBufferSize = 1
                * numVertexDimensions
                * indicesPerSprite
                * sizeof(GLfloat);
        glBufferData(
                GL_ARRAY_BUFFER,
                positionBufferSize,
                NULL,
                GL_DYNAMIC_DRAW);

        // Initialize tex co-ord buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        int texCoordBufferSize = 1
                * numTexCoordDimensions
                * indicesPerSprite
                * sizeof(GLfloat);
        glVertexAttribPointer(
                Shaders::texCoordAttribIndex,
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

        // Initialize index buffer - this should never need to change
        std::vector<GLuint> indexData;
        indexData.reserve(1 * indicesPerSprite);
        for (int i = 0; i < 1; i++) {

            unsigned int startIndex = i * numVerticesPerSprite;

            indexData.push_back(startIndex + 3);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 1);
            indexData.push_back(startIndex);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                1 * indicesPerSprite * sizeof(GLuint),
                indexData.data(),
                GL_STATIC_DRAW);

        // Enable vertex attributes
        glEnableVertexAttribArray(Shaders::vertexAttribIndex);
        glEnableVertexAttribArray(Shaders::texCoordAttribIndex);

        // Upload data to the GPU

        // Define vertex positions
        float width = 8;
        float height = 0.5f;
        float x1 = -0.5f;
        float y1 = -0.5f;
        float x2 = x1 + width;
        float y2 = y1 + height;
        float z = 0;
        std::vector<GLfloat> vertexData = {
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
        };

        // Determine texture co-ordinates
        const float tx1 = 0;
        const float tx2 = 1;
        const float ty1 = 1;
        const float ty2 = 0;
        std::vector<GLfloat> texCoords = {
            tx1, ty1,
            tx2, ty1,
            tx2, ty2,
            tx1, ty2
        };

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                vertexData.size() * sizeof(GLfloat),
                vertexData.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                texCoords.size() * sizeof(GLfloat),
                texCoords.data());
    }

    void TextRenderer::render(Texture& texture) {
        // For now just try to render the whole texture

        glViewport(0, 0, 800, 600);

        // Use screen texture shader
        glUseProgram(Shaders::screenShader.programId);

        // Use textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getId());

        // Bind vertex array
        glBindVertexArray(vao);

        // Render
        glDrawElements(
                GL_TRIANGLE_FAN,
                4,
                GL_UNSIGNED_INT,
                nullptr);
    }

}  // namespace Rival
