#include "pch.h"

#include "MapBorderRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

#include "MathUtils.h"
#include "Palette.h"
#include "Shaders.h"

namespace Rival {

    MapBorderRenderer::MapBorderRenderer(
            Race race, int mapWidth, int mapHeight, const Spritesheet& spritesheet, const Texture& paletteTexture)
        : paletteTexture(paletteTexture)
        ,
        // This is calculated as follows:
        //  - 1 segment per edge tile
        //  - Minus 4 (since the corners are duplicated by each edge)
        //  - Plus 2 (some overlap at the bottom corners is actually required
        //      to prevent gaps in maps with an odd height)
        maxSegmentsToRender(2 * (mapWidth + mapHeight) - 2)
        , renderable { spritesheet, maxSegmentsToRender }
    {

        // The map border never changes, so we set the buffers here and never
        // touch them again.

        // Create buffers to hold all our vertex data
        int numVertices = maxSegmentsToRender * SpriteRenderable::numVerticesPerSprite;
        int positionDataSize = numVertices * SpriteRenderable::numVertexDimensions;
        int texCoordDataSize = numVertices * SpriteRenderable::numTexCoordDimensions;
        std::vector<GLfloat> positions;
        std::vector<GLfloat> texCoords;
        positions.reserve(positionDataSize);
        texCoords.reserve(texCoordDataSize);

        // Add data to buffers
        int raceOffset = race == Race::Greenskin ? 8 : 0;
        createLeftEdge(positions, texCoords, raceOffset, mapHeight);
        createTopEdge(positions, texCoords, raceOffset, mapWidth);
        createRightEdge(positions, texCoords, raceOffset, mapWidth, mapHeight);
        createBottomEdge(positions, texCoords, raceOffset, mapWidth, mapHeight);
        createCorners(positions, texCoords, raceOffset, mapWidth, mapHeight);

        // Bind vertex array
        glBindVertexArray(renderable.getVao());

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(GLfloat), positions.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
        glBufferSubData(GL_ARRAY_BUFFER, 0, texCoords.size() * sizeof(GLfloat), texCoords.data());
    }

    void MapBorderRenderer::render() const
    {

        // Use textures
        glActiveTexture(GL_TEXTURE0 + 0);  // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
        glActiveTexture(GL_TEXTURE0 + 1);  // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

        // Bind vertex array
        glBindVertexArray(renderable.getVao());

        // Render
        glDrawElements(
                renderable.getDrawMode(), numSegments * renderable.getIndicesPerSprite(), GL_UNSIGNED_INT, nullptr);
    }

    void MapBorderRenderer::createLeftEdge(
            std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords, int raceOffset, int mapHeight)
    {
        for (int tileY = 1; tileY < mapHeight; ++tileY)
        {
            addDataToBuffers(positions, texCoords, raceOffset + txIndexLeft, 0, static_cast<float>(tileY));
            numSegments++;
        }
    }

    void MapBorderRenderer::createTopEdge(
            std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords, int raceOffset, int mapWidth)
    {
        for (int tileX = 1; tileX < mapWidth - 1; ++tileX)
        {
            addDataToBuffers(positions, texCoords, raceOffset + txIndexTop, static_cast<float>(tileX), 0);
            numSegments++;
        }
    }

    void MapBorderRenderer::createRightEdge(
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords,
            int raceOffset,
            int mapWidth,
            int mapHeight)
    {
        for (int tileY = 1; tileY < mapHeight; ++tileY)
        {
            addDataToBuffers(
                    positions, texCoords, raceOffset + txIndexRight, mapWidth - 1.0f, static_cast<float>(tileY));
            numSegments++;
        }
    }

    void MapBorderRenderer::createBottomEdge(
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords,
            int raceOffset,
            int mapWidth,
            int mapHeight)
    {
        for (int tileX = 1; tileX < mapWidth - 1; ++tileX)
        {
            addDataToBuffers(
                    positions, texCoords, raceOffset + txIndexBottom, static_cast<float>(tileX), mapHeight - 0.5f);
            numSegments++;
        }
    }

    void MapBorderRenderer::createCorners(
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords,
            int raceOffset,
            int mapWidth,
            int mapHeight)
    {

        addDataToBuffers(positions, texCoords, raceOffset + txIndexTopLeft, 0, 0);
        addDataToBuffers(positions, texCoords, raceOffset + txIndexTopRight, mapWidth - 1.0f, 0);
        addDataToBuffers(positions, texCoords, raceOffset + txIndexBottomRight, mapWidth - 1.0f, mapHeight - 0.5f);
        addDataToBuffers(positions, texCoords, raceOffset + txIndexBottomLeft, 0, mapHeight - 0.5f);

        numSegments += 4;
    }

    void MapBorderRenderer::addDataToBuffers(
            std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords, int txIndex, float tileX, float tileY)
    {

        // Define vertex positions
        float width = static_cast<float>(RenderUtils::tileSpriteWidthPx);
        float height = static_cast<float>(RenderUtils::tileSpriteHeightPx);
        float x1 = RenderUtils::cameraToPx_X(tileX);
        float y1 = RenderUtils::cameraToPx_Y(tileY);
        float x2 = x1 + width;
        float y2 = y1 + height;
        float z = RenderUtils::zMapBorder;
        std::vector<GLfloat> thisVertexData = { x1, y1, z, x2, y1, z, x2, y2, z, x1, y2, z };

        // Determine texture co-ordinates
        std::vector<GLfloat> thisTexCoords = renderable.spritesheet.getTexCoords(txIndex);

        // Copy this tile's data to the main buffers
        positions.insert(positions.cend(), thisVertexData.cbegin(), thisVertexData.cend());
        texCoords.insert(texCoords.cend(), thisTexCoords.cbegin(), thisTexCoords.cend());
    }

}  // namespace Rival
