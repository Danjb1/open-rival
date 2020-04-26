#include "pch.h"
#include "MapBorderRenderer.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MathUtils.h"
#include "Palette.h"
#include "Shaders.h"

namespace Rival {

    MapBorderRenderer::MapBorderRenderer(
            const Spritesheet& spritesheet,
            const Texture& paletteTexture) :
        paletteTexture(paletteTexture),
        renderable{ spritesheet, maxSegmentsToRender } {}

    void MapBorderRenderer::render(
            const Race race,
            const Camera& camera,
            int mapWidth,
            int mapHeight) {

        // Use textures
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

        // Create buffers to hold all our vertex data
        int numVertices = maxSegmentsToRender * SpriteRenderable::numVerticesPerSprite;
        int positionDataSize = numVertices * SpriteRenderable::numVertexDimensions;
        int texCoordDataSize = numVertices * SpriteRenderable::numTexCoordDimensions;
        std::vector<GLfloat> positions;
        std::vector<GLfloat> texCoords;
        positions.reserve(positionDataSize);
        texCoords.reserve(texCoordDataSize);

        // Add data to buffers
        int renderedSegments = 0;
        createLeftEdge(renderedSegments, positions, texCoords, mapHeight);
        createTopEdge(renderedSegments, positions, texCoords, mapWidth);
        createRightEdge(renderedSegments, positions, texCoords, mapWidth, mapHeight);
        createBottomEdge(renderedSegments, positions, texCoords, mapWidth, mapHeight);

        // Bind vertex array
        glBindVertexArray(renderable.getVao());

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                positions.size() * sizeof(GLfloat),
                positions.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                texCoords.size() * sizeof(GLfloat),
                texCoords.data());

        // Render
        glDrawElements(
                renderable.getDrawMode(),
                renderedSegments * renderable.getIndicesPerSprite(),
                GL_UNSIGNED_INT,
                nullptr);
    }

    void MapBorderRenderer::createLeftEdge(
            int& renderedSegments,
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords,
            int mapHeight) {
        for (int tileY = 1; tileY <= mapHeight - 1; tileY++) {
            addDataToBuffers(positions, texCoords, 3, 0, tileY);
            renderedSegments++;
        }
    }

    void MapBorderRenderer::createTopEdge(
            int& renderedSegments,
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords,
            int mapWidth) {
        for (int tileX = 1; tileX <= mapWidth - 1; tileX++) {
            addDataToBuffers(positions, texCoords, 0, tileX, 0);
            renderedSegments++;
        }
    }

    void MapBorderRenderer::createRightEdge(
            int& renderedSegments,
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords,
            int mapWidth,
            int mapHeight) {
        for (int tileY = 1; tileY <= mapHeight - 1; tileY++) {
            addDataToBuffers(positions, texCoords, 1, mapWidth - 1, tileY);
            renderedSegments++;
        }
    }

    void MapBorderRenderer::createBottomEdge(
            int& renderedSegments,
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords,
            int mapWidth,
            int mapHeight) {
        for (int tileX = 1; tileX <= mapWidth - 1; tileX++) {
            addDataToBuffers(positions, texCoords, 2, tileX, mapHeight - 1);
            renderedSegments++;
        }
    }

    void MapBorderRenderer::addDataToBuffers(
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords,
            int txIndex,
            int tileX,
            int tileY) {

        // Define vertex positions
        float width = static_cast<float>(RenderUtils::tileSpriteWidthPx);
        float height = static_cast<float>(RenderUtils::tileSpriteHeightPx);
        float x1 = RenderUtils::worldToPx_X(static_cast<float>(tileX));
        float y1 = RenderUtils::worldToPx_Y(static_cast<float>(tileY));
        float x2 = x1 + width;
        float y2 = y1 + height;
        std::vector<GLfloat> thisVertexData = {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        };

        // Determine texture co-ordinates
        std::vector<GLfloat> thisTexCoords =
                renderable.spritesheet.getTexCoords(txIndex);

        // Copy this tile's data to the main buffers
        positions.insert(
                positions.end(),
                thisVertexData.begin(),
                thisVertexData.end());
        texCoords.insert(
                texCoords.end(),
                thisTexCoords.begin(),
                thisTexCoords.end());
    }

}
