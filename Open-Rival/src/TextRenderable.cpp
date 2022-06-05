#include "pch.h"
#include "TextRenderable.h"

#include "Shaders.h"

namespace Rival {

    const Color TextRenderable::defaultColor = Color::makeRgb(255, 221, 65);
    const Color TextRenderable::highlightColor = Color::makeRgb(255, 43, 40);

    const float TextRenderable::shadowOffsetX = 0.0f;
    const float TextRenderable::shadowOffsetY = 1.0f;

    TextRenderable::TextRenderable(TextSpan span, TextProperties props, float x, float y)
        : spans({ span })
        , props(props)
        , x(x)
        , y(y)
        , numChars(0)
        , numVisibleChars(0) {
        countChars();
        init();
    }

    TextRenderable::TextRenderable(std::vector<TextSpan> spans, const TextProperties props, float x, float y)
        : spans(spans)
        , props(props)
        , x(x)
        , y(y)
        , numChars(0)
        , numVisibleChars(0) {
        countChars();
        init();
    }

    TextRenderable::TextRenderable(int maxChars, TextProperties props, float x, float y)
        : props(props)
        , x(x)
        , y(y)
        , numChars(maxChars)
        , numVisibleChars(maxChars) {
        init();
    }

    void TextRenderable::init() {
        // Generate VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generate VBOs
        glGenBuffers(1, &positionVbo);
        glGenBuffers(1, &texCoordVbo);
        glGenBuffers(1, &colorVbo);
        glGenBuffers(1, &ibo);

        int numLayers = getNumLayers();

        // Initialize position buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glVertexAttribPointer(
                Shaders::vertexAttribIndex,
                numVertexDimensions,
                GL_FLOAT,
                GL_FALSE,
                numVertexDimensions * sizeof(GLfloat),
                nullptr);
        int positionBufferSize =
                numVisibleChars * numLayers * numVertexDimensions * numIndicesPerChar * sizeof(GLfloat);
        glBufferData(GL_ARRAY_BUFFER, positionBufferSize, NULL, GL_STATIC_DRAW);

        // Initialize tex co-ord buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        int texCoordBufferSize =
                numVisibleChars * numLayers * numTexCoordDimensions * numIndicesPerChar * sizeof(GLfloat);
        glVertexAttribPointer(
                Shaders::texCoordAttribIndex,
                numTexCoordDimensions,
                GL_FLOAT,
                GL_FALSE,
                numTexCoordDimensions * sizeof(GLfloat),
                nullptr);
        glBufferData(GL_ARRAY_BUFFER, texCoordBufferSize, NULL, GL_STATIC_DRAW);

        // Initialize color buffer with empty data
        glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
        int colorBufferSize = numVisibleChars * numLayers * numColorDimensions * numIndicesPerChar * sizeof(GLfloat);
        glVertexAttribPointer(
                Shaders::colorAttribIndex,
                numColorDimensions,
                GL_FLOAT,
                GL_FALSE,
                numColorDimensions * sizeof(GLfloat),
                nullptr);
        glBufferData(GL_ARRAY_BUFFER, colorBufferSize, NULL, GL_STATIC_DRAW);

        // Initialize index buffer with empty data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        int numIndices = numVisibleChars * numLayers * numIndicesPerChar * sizeof(GLuint);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices, NULL, GL_STATIC_DRAW);

        // Enable vertex attributes
        glEnableVertexAttribArray(Shaders::vertexAttribIndex);
        glEnableVertexAttribArray(Shaders::texCoordAttribIndex);
        glEnableVertexAttribArray(Shaders::colorAttribIndex);
    }

    void TextRenderable::countChars() {
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
        return props.font->getTexture().getId();
    }

    int TextRenderable::getNumLayers() const {
        return props.hasShadow ? numLayersWithShadow : numLayersWithoutShadow;
    }

    void TextRenderable::setTextSpan(TextSpan newSpan) {
        setTextSpans({ newSpan });
    }

    void TextRenderable::setTextSpans(std::vector<TextSpan> newSpans) {
        spans = newSpans;
    }

}  // namespace Rival
