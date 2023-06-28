#include "TextRenderable.h"

#include "Shaders.h"

namespace Rival {

const Color TextRenderable::defaultColor = Color::makeRgb(255, 221, 65);
const Color TextRenderable::highlightColor = Color::makeRgb(255, 43, 40);

TextRenderable::TextRenderable(TextSpan span, TextProperties props, float x, float y)
    : spans({ span })
    , props(props)
    , x(x)
    , y(y)
    , numChars(0)
    , numVisibleChars(0)
{
    countChars();
    init();
}

TextRenderable::TextRenderable(std::vector<TextSpan> spans, const TextProperties props, float x, float y)
    : spans(spans)
    , props(props)
    , x(x)
    , y(y)
    , numChars(0)
    , numVisibleChars(0)
{
    countChars();
    init();
}

TextRenderable::TextRenderable(int maxChars, TextProperties props, float x, float y)
    : props(props)
    , x(x)
    , y(y)
    , numChars(maxChars)
    , numVisibleChars(maxChars)
{
    init();
}

TextRenderable::TextRenderable(TextRenderable&& other) noexcept
    : spans(other.spans)
    , props(other.props)
    , x(other.x)
    , y(other.y)
    , numChars(other.numChars)
    , numVisibleChars(other.numVisibleChars)

    // Reset the source object so its destructor is harmless
    , vao(std::exchange(other.vao, 0))
    , positionVbo(std::exchange(other.positionVbo, 0))
    , texCoordVbo(std::exchange(other.texCoordVbo, 0))
    , colorVbo(std::exchange(other.colorVbo, 0))
    , ibo(std::exchange(other.ibo, 0))
{
}

TextRenderable::~TextRenderable()
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
        glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
        glDeleteBuffers(1, &colorVbo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glDeleteBuffers(1, &ibo);
    }
    glDeleteVertexArrays(1, &vao);
    glBindVertexArray(0);
}

void TextRenderable::init()
{
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
    int positionBufferSize = numVisibleChars * numLayers * numVertexDimensions * numIndicesPerChar * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, positionBufferSize, NULL, GL_STATIC_DRAW);

    // Initialize tex co-ord buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    int texCoordBufferSize = numVisibleChars * numLayers * numTexCoordDimensions * numIndicesPerChar * sizeof(GLfloat);
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

void TextRenderable::countChars()
{
    numChars = 0;
    numVisibleChars = 0;

    for (TextSpan span : spans)
    {
        for (char c : span.text)
        {
            ++numChars;

            if (c != ' ')
            {
                ++numVisibleChars;
            }
        }
    }
}

GLuint TextRenderable::getTextureId() const
{
    return props.font->getTexture()->getId();
}

int TextRenderable::getNumLayers() const
{
    return props.hasShadow ? numLayersWithShadow : numLayersWithoutShadow;
}

void TextRenderable::setTextSpan(TextSpan newSpan)
{
    setTextSpans({ newSpan });
}

void TextRenderable::setTextSpans(std::vector<TextSpan> newSpans)
{
    spans = newSpans;
    countChars();
    dirty = true;
}

}  // namespace Rival
