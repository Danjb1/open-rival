#include "pch.h"
#include "TileRenderer.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

#include "Palette.h"
#include "RenderUtils.h"
#include "Rival.h"
#include "Shaders.h"
#include "SpriteRenderable.h"

namespace Rival {

    TileRenderer::TileRenderer(
            const Spritesheet& spritesheet,
            const Texture& paletteTexture) :
        paletteTexture(paletteTexture),
        renderable{ spritesheet, maxTilesToRender } {}

    void TileRenderer::render(
            Camera& camera,
            std::vector<Tile>& tiles,
            int mapWidth,
            int mapHeight) {

        // Find the first visible tiles
        int minX = static_cast<int>(camera.getLeft());
        int minY = static_cast<int>(camera.getTop());

        // Find the last visible tiles
        int maxX = minX + static_cast<int>(camera.getWidth());
        int maxY = minY + static_cast<int>(camera.getHeight());

        // Use textures
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

        // Create buffers to hold all our tile data
        int numVertices = tiles.size() * SpriteRenderable::numVerticesPerSprite;
        int positionDataSize = numVertices * SpriteRenderable::numVertexDimensions;
        int texCoordDataSize = numVertices * SpriteRenderable::numTexCoordDimensions;
        std::vector<GLfloat> positions;
        std::vector<GLfloat> texCoords;
        positions.reserve(positionDataSize);
        texCoords.reserve(texCoordDataSize);

        // Add data to buffers
        for (int tileY = minY; tileY < maxY; tileY++) {
            for (int tileX = minX; tileX < maxX; tileX++) {

                auto const& tile = tiles[tileY * mapWidth + tileX];
                const int txIndex = tile.txIndex;

                // Define vertex positions
                //    0------1
                //    | \    |
                //    |   \..|
                //    3----- 2
                float width = static_cast<float>(RenderUtils::tileSpriteWidthPx);
                float height = static_cast<float>(RenderUtils::tileSpriteHeightPx);
                float x1 = static_cast<float>(RenderUtils::getRenderPosX(tileX));
                float y1 = static_cast<float>(RenderUtils::getRenderPosY(tileX, tileY));
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
                tiles.size() * renderable.getIndicesPerSprite(),
                GL_UNSIGNED_INT,
                nullptr);
    }

}
