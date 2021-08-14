#include "pch.h"
#include "TextRenderable.h"

#include "Shaders.h"

namespace Rival {

    TextRenderable::TextRenderable(
            const std::string text, const Font& font, float x, float y)
        : text(text),
          font(&font),
          x(x),
          y(y) {

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
        int positionBufferSize = text.length()
                * numVertexDimensions
                * numIndicesPerChar
                * sizeof(GLfloat);
        glBufferData(
                GL_ARRAY_BUFFER,
                positionBufferSize,
                NULL,
                GL_STATIC_DRAW);

        // Initialize tex co-ord buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        int texCoordBufferSize = text.length()
                * numTexCoordDimensions
                * numIndicesPerChar
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
                GL_STATIC_DRAW);

        // Initialize index buffer with empty data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                text.length() * numIndicesPerChar * sizeof(GLuint),
                NULL,
                GL_STATIC_DRAW);

        // Enable vertex attributes
        glEnableVertexAttribArray(Shaders::vertexAttribIndex);
        glEnableVertexAttribArray(Shaders::texCoordAttribIndex);
    }

    GLuint TextRenderable::getTextureId() const {
        return font->getTexture().getId();
    }

}  // namespace Rival
