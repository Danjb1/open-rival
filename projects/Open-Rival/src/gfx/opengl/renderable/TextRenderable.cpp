#include "gfx/opengl/renderable/TextRenderable.h"

#include <utility>  // std::exchange

#include "gfx/Font.h"
#include "gfx/opengl/Shaders.h"
#include "utils/LogUtils.h"

namespace Rival {

const Color TextRenderable::defaultColor = Color::makeRgb(255, 221, 65);
const Color TextRenderable::highlightColor = Color::makeRgb(255, 43, 40);

TextRenderable::TextRenderable()
{
    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs
    glGenBuffers(1, &positionVbo);
    glGenBuffers(1, &texCoordVbo);
    glGenBuffers(1, &colorVbo);
    glGenBuffers(1, &ibo);

    // Describe the vertex attributes (the buffers are sized in setText)
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(Shaders::vertexAttribIndex,
            numVertexDimensions,
            GL_FLOAT,
            GL_FALSE,
            numVertexDimensions * sizeof(GLfloat),
            nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    glVertexAttribPointer(Shaders::texCoordAttribIndex,
            numTexCoordDimensions,
            GL_FLOAT,
            GL_FALSE,
            numTexCoordDimensions * sizeof(GLfloat),
            nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glVertexAttribPointer(Shaders::colorAttribIndex,
            numColorDimensions,
            GL_FLOAT,
            GL_FALSE,
            numColorDimensions * sizeof(GLfloat),
            nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Enable vertex attributes
    glEnableVertexAttribArray(Shaders::vertexAttribIndex);
    glEnableVertexAttribArray(Shaders::texCoordAttribIndex);
    glEnableVertexAttribArray(Shaders::colorAttribIndex);
}

TextRenderable::TextRenderable(TextRenderable&& other) noexcept
    // Reset the source object so its destructor is harmless
    : vao(std::exchange(other.vao, 0))
    , positionVbo(std::exchange(other.positionVbo, 0))
    , texCoordVbo(std::exchange(other.texCoordVbo, 0))
    , colorVbo(std::exchange(other.colorVbo, 0))
    , ibo(std::exchange(other.ibo, 0))
    , props(std::move(other.props))
    , numIndices(other.numIndices)
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

void TextRenderable::setText(const std::vector<TextSpan>& spans, const TextProperties& newProps, float x, float y)
{
    props = newProps;
    const Font& font = *props.font;
    const float glyphScale = font.getGlyphScale(props.scale);

    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> texCoordData;
    std::vector<GLfloat> colorData;
    std::vector<GLuint> indexData;
    int numCharsAdded = 0;

    // Each layer is a full copy of the text; the shadow (if any) is drawn first so that it appears underneath
    for (int layer = 0; layer < getNumLayers(); ++layer)
    {
        const bool isShadow = isShadowLayer(layer);
        const float layerX = isShadow ? x + shadowOffsetX : x;
        const float layerY = isShadow ? y + shadowOffsetY : y;
        float penX = layerX;

        for (const TextSpan& span : spans)
        {
            const Color color = isShadow ? Color::makeRgb(0, 0, 0) : span.color;

            for (char c : span.text)
            {
                const CharData* charData = font.getCharData(c);
                if (!charData)
                {
                    LOG_WARN("Trying to render unsupported character: {}", c);
                    continue;
                }

                if (c == ' ')
                {
                    // Nothing to render for spaces
                    penX += charData->advance * glyphScale;
                    continue;
                }

                const float x1 = penX + charData->bearing.x * glyphScale;
                const float y1 = layerY - charData->bearing.y * glyphScale;
                const float x2 = x1 + charData->size.x * glyphScale;
                const float y2 = y1 + charData->size.y * glyphScale;
                const float z = 0.f;

                /* clang-format off */
                vertexData.insert(vertexData.end(), {
                    x1, y1, z,
                    x2, y1, z,
                    x2, y2, z,
                    x1, y2, z
                });
                /* clang-format on */

                const float tx1 = charData->txCoords[0];
                const float ty1 = charData->txCoords[1];
                const float tx2 = charData->txCoords[2];
                const float ty2 = charData->txCoords[3];

                /* clang-format off */
                texCoordData.insert(texCoordData.end(), {
                    tx1, ty1,
                    tx2, ty1,
                    tx2, ty2,
                    tx1, ty2
                });
                /* clang-format on */

                for (int vertex = 0; vertex < numVerticesPerChar; ++vertex)
                {
                    colorData.insert(colorData.end(), { color.r, color.g, color.b });
                }

                const GLuint startIndex = numCharsAdded * numVerticesPerChar;
                indexData.insert(indexData.end(),
                        { startIndex, startIndex + 1, startIndex + 2, startIndex + 2, startIndex + 3, startIndex });

                penX += charData->advance * glyphScale;
                ++numCharsAdded;
            }
        }
    }

    numIndices = static_cast<int>(indexData.size());

    // The amount of text may have changed, so we recreate each buffer at the new size
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    glBufferData(GL_ARRAY_BUFFER, texCoordData.size() * sizeof(GLfloat), texCoordData.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(GLfloat), colorData.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_DYNAMIC_DRAW);
}

void TextRenderable::draw() const
{
    if (numIndices == 0)
    {
        // Nothing to draw (and we may not have a font yet)
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, props.font->getTexture()->getId());

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

int TextRenderable::getNumLayers() const
{
    return props.hasShadow ? numLayersWithShadow : numLayersWithoutShadow;
}

bool TextRenderable::isShadowLayer(int layer) const
{
    return props.hasShadow && layer == shadowLayerIndex;
}

}  // namespace Rival
