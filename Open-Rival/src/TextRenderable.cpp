#include "pch.h"
#include "TextRenderable.h"

#include "Shaders.h"

namespace Rival {

    const Color TextRenderable::defaultColor = Color::makeRgb(255, 221, 65);
    const Color TextRenderable::highlightColor = Color::makeRgb(255, 43, 40);

    TextRenderable::TextRenderable(
            std::vector<TextSpan> spans,
            const TextProperties props,
            float x,
            float y)
        : spans(spans),
          props(props),
          x(x),
          y(y),
          numChars(0),
          numVisibleChars(0) {

        refresh();

        // Generate VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generate VBOs
        glGenBuffers(1, &positionVbo);
        glGenBuffers(1, &texCoordVbo);
        glGenBuffers(1, &colorVbo);
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
        int positionBufferSize = numVisibleChars
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
        int texCoordBufferSize = numVisibleChars
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

        // Initialize color buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
        int colorBufferSize = numVisibleChars
                * numColorDimensions
                * numIndicesPerChar
                * sizeof(GLfloat);
        glVertexAttribPointer(
                Shaders::colorAttribIndex,
                numColorDimensions,
                GL_FLOAT,
                GL_FALSE,
                numColorDimensions * sizeof(GLfloat),
                nullptr);
        glBufferData(
                GL_ARRAY_BUFFER,
                colorBufferSize,
                NULL,
                GL_STATIC_DRAW);

        // Initialize index buffer with empty data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                numVisibleChars * numIndicesPerChar * sizeof(GLuint),
                NULL,
                GL_STATIC_DRAW);

        // Enable vertex attributes
        glEnableVertexAttribArray(Shaders::vertexAttribIndex);
        glEnableVertexAttribArray(Shaders::texCoordAttribIndex);
        glEnableVertexAttribArray(Shaders::colorAttribIndex);
    }

    void TextRenderable::refresh() {
        numChars = 0;
        numVisibleChars = 0;

        for (TextSpan span : spans) {
            for (char c : span.text) {
                ++numChars;

                if (c != ' ') {
                    ++numVisibleChars;
                }
            }
        }
    }

    GLuint TextRenderable::getTextureId() const {
        return props.font.getTexture().getId();
    }

}  // namespace Rival
