#include "pch.h"
#include "TextRenderer.h"

#include "Framebuffer.h"
#include "RenderUtils.h"
#include "Shaders.h"

namespace Rival {

    void TextRenderer::render(const TextRenderable& textRenderable) {
        glViewport(0, 0, 800, 600);

        // Use screen texture shader
        glUseProgram(Shaders::screenShader.programId);

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
                * textRenderable.getText().length();
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

        std::string text = textRenderable.getText();
        const Font* font = textRenderable.getFont();

        // Create buffers
        std::vector<GLfloat> vertexData;
        std::vector<GLfloat> texCoords;
        std::vector<GLuint> indexData;

        // Reserve space upfront
        int numChars = text.length();
        vertexData.reserve(numChars
                * TextRenderable::numVertexDimensions
                * TextRenderable::numVerticesPerChar);
        texCoords.reserve(numChars * TextRenderable::numTexCoordDimensions
                * TextRenderable::numVerticesPerChar);
        indexData.reserve(numChars * TextRenderable::numIndicesPerChar);

        int charsAdded = 0;
        float x = textRenderable.getX();
        float y = textRenderable.getY();

        // Add characters to buffers
        for (char c : text) {
            if (c == ' ') {
                x += 200;
                continue;
            }

            const CharData* charData = font->getCharData(c);

            if (!charData) {
                std::cout << "Trying to render unsupported character: "
                          << c
                          << "\n";
                continue;
            }

            // Define vertex positions
            float width = static_cast<float>(charData->size.x);
            float height = static_cast<float>(charData->size.y);
            float x1 = static_cast<float>(x + charData->bearing.x);
            float y1 = static_cast<float>(y + charData->bearing.y);
            x1 /= 1000;     // TMP (no MVP matrix!)
            y1 /= 1000;     // TMP (no MVP matrix!)
            width /= 100;   // TMP (no MVP matrix!)
            height /= 100;  // TMP (no MVP matrix!)
            float x2 = x1 + width;
            float y2 = y1 + height;
            std::swap(y1, y2);  // TMP (no MVP matrix!)
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

            // Determine indices
            unsigned int startIndex =
                    charsAdded * TextRenderable::numVerticesPerChar;
            indexData.push_back(startIndex);
            indexData.push_back(startIndex + 1);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 3);
            indexData.push_back(startIndex + 0);

            x += charData->advance * 8;  // TMP (no MVP matrix!)
            ++charsAdded;
        }

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, textRenderable.getPositionVbo());
        int positionBufferSize =
                vertexData.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                positionBufferSize,
                vertexData.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, textRenderable.getTexCoordVbo());
        int texCoordBufferSize =
                texCoords.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                texCoordBufferSize,
                texCoords.data());

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
