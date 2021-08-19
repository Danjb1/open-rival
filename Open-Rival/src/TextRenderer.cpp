#include "pch.h"
#include "TextRenderer.h"

#include "Framebuffer.h"
#include "RenderUtils.h"

namespace Rival {

    void TextRenderer::render(const TextRenderable& textRenderable) const {
        // Use textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textRenderable.getTextureId());

        // Bind vertex array
        glBindVertexArray(textRenderable.getVao());

        // Update the data on the GPU
        if (needsUpdate(textRenderable)) {
            sendDataToGpu(textRenderable);
        }

        // Render
        GLsizei numIndices = TextRenderable::numIndicesPerChar
                * textRenderable.getNumVisibleChars();
        glDrawElements(
                GL_TRIANGLES,
                numIndices,
                GL_UNSIGNED_INT,
                nullptr);
    }

    bool TextRenderer::needsUpdate(const TextRenderable& textRenderable) const {
        return textRenderable.dirty;
    }

    void TextRenderer::sendDataToGpu(
            const TextRenderable& textRenderable) const {

        std::vector<TextSpan> spans = textRenderable.getTextSpans();
        const Font* font = textRenderable.getFont();
        float scale = textRenderable.getScale()
                * font->getDefaultSize() / Font::fontHeight;

        // Create buffers
        std::vector<GLfloat> vertexData;
        std::vector<GLfloat> texCoords;
        std::vector<GLfloat> colors;
        std::vector<GLuint> indexData;

        // Reserve space upfront
        int numVisibleChars = textRenderable.getNumVisibleChars();
        vertexData.reserve(numVisibleChars
                * TextRenderable::numVertexDimensions
                * TextRenderable::numVerticesPerChar);
        texCoords.reserve(numVisibleChars
                * TextRenderable::numTexCoordDimensions
                * TextRenderable::numVerticesPerChar);
        colors.reserve(numVisibleChars
                * TextRenderable::numColorDimensions
                * TextRenderable::numVerticesPerChar);
        indexData.reserve(numVisibleChars * TextRenderable::numIndicesPerChar);

        int charsAdded = 0;
        float x = textRenderable.getX();
        float y = textRenderable.getY();

        // Add characters to buffers
        for (TextSpan span : spans) {
            for (char c : span.text) {
                const CharData* charData = font->getCharData(c);

                if (!charData) {
                    std::cout << "Trying to render unsupported character: "
                              << c
                              << "\n";
                    continue;
                }

                if (c == ' ') {
                    // Nothing to render for spaces
                    x += charData->advance * scale;
                    continue;
                }

                // Define vertex positions
                float width = static_cast<float>(charData->size.x) * scale;
                float height = static_cast<float>(charData->size.y) * scale;
                float x1 = static_cast<float>(x + charData->bearing.x * scale);
                float y1 = static_cast<float>(y - charData->bearing.y * scale);
                float x2 = x1 + width;
                float y2 = y1 + height;
                float z = 0;
                std::vector<GLfloat> newVertexData = {
                    x1, y1, z,
                    x2, y1, z,
                    x2, y2, z,
                    x1, y2, z
                };
                vertexData.insert(
                        vertexData.end(),
                        newVertexData.begin(),
                        newVertexData.end());

                // Determine texture co-ordinates
                float tx1 = charData->txCoords[0];
                float ty1 = charData->txCoords[1];
                float tx2 = charData->txCoords[2];
                float ty2 = charData->txCoords[3];
                std::vector<GLfloat> newTexCoords = {
                    tx1, ty1,
                    tx2, ty1,
                    tx2, ty2,
                    tx1, ty2
                };
                texCoords.insert(
                        texCoords.end(),
                        newTexCoords.begin(),
                        newTexCoords.end());

                // Determine colors
                float r = span.color.r;
                float g = span.color.g;
                float b = span.color.b;
                std::vector<GLfloat> newColors = {
                    /* clang-format off */
                    r, g, b,
                    r, g, b,
                    r, g, b,
                    r, g, b,
                    /* clang-format on */
                };
                colors.insert(
                        colors.end(),
                        newColors.begin(),
                        newColors.end());

                // Determine indices
                unsigned int startIndex =
                        charsAdded * TextRenderable::numVerticesPerChar;
                indexData.push_back(startIndex);
                indexData.push_back(startIndex + 1);
                indexData.push_back(startIndex + 2);
                indexData.push_back(startIndex + 2);
                indexData.push_back(startIndex + 3);
                indexData.push_back(startIndex + 0);

                x += charData->advance * scale;
                ++charsAdded;
            }
        }

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, textRenderable.getPositionVbo());
        int positionBufferSize = vertexData.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                positionBufferSize,
                vertexData.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, textRenderable.getTexCoordVbo());
        int texCoordBufferSize = texCoords.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                texCoordBufferSize,
                texCoords.data());

        // Upload color data
        glBindBuffer(GL_ARRAY_BUFFER, textRenderable.getColorVbo());
        int colorBufferSize = colors.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                colorBufferSize,
                colors.data());

        // Upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textRenderable.getIbo());
        int indexBufferSize = indexData.size() * sizeof(GLuint);
        glBufferSubData(
                GL_ELEMENT_ARRAY_BUFFER,
                0,
                indexBufferSize,
                indexData.data());

        // Clear the dirty flag now that the GPU is up to date
        textRenderable.dirty = false;
    }

}  // namespace Rival
