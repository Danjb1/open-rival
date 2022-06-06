#include "pch.h"

#include "TileRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)
#include <algorithm>
#include <map>

#include "MathUtils.h"
#include "Palette.h"
#include "Shaders.h"

namespace Rival {

    TileRenderer::TileRenderer(const Spritesheet& spritesheet, const Texture& paletteTexture)
        : paletteTexture(paletteTexture)
        , renderable { spritesheet, maxTilesToRender }
    {
    }

    /**
     * Renders all tiles visible to the camera.
     *
     * Note that while this is very slow in Debug mode, performance is not an
     * issue in Release mode.
     */
    void TileRenderer::render(const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight) const
    {

        // Use textures
        glActiveTexture(GL_TEXTURE0 + 0);  // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
        glActiveTexture(GL_TEXTURE0 + 1);  // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

        // Bind vertex array
        glBindVertexArray(renderable.getVao());

        // Update the data on the GPU
        if (needsUpdate())
        {
            sendDataToGpu(camera, tiles, mapWidth, mapHeight);
        }

        // Render
        glDrawElements(
                renderable.getDrawMode(), tiles.size() * renderable.getIndicesPerSprite(), GL_UNSIGNED_INT, nullptr);
    }

    bool TileRenderer::needsUpdate() const
    {
        // Currently we update the buffers every frame.
        // Later, we could optimise this by updating only when:
        //   1) Camera has moved
        //   2) Zoom level has changed
        //   3) A tile has changed
        return true;
    }

    void
    TileRenderer::sendDataToGpu(const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight) const
    {

        // Find the first visible tiles.
        // We subtract 1 because we need to start drawing offscreen, to account
        // for tiles that are partially visible to the camera.
        int minX = static_cast<int>(camera.getLeft()) - 1;
        int minY = static_cast<int>(camera.getTop()) - 1;

        // Keep within bounds
        if (minX < 0)
        {
            minX = 0;
        }
        if (minY < 0)
        {
            minY = 0;
        }

        // Determine the number of tiles to draw.
        // We add 3 to account for partially visible tiles either side of the
        // screen, and because we started drawing offscreen.
        int numTilesX = static_cast<int>(ceil(camera.getWidth())) + 3;
        int numTilesY = static_cast<int>(ceil(camera.getHeight())) + 3;

        // Find the last visible tiles
        int maxX = minX + numTilesX;
        int maxY = minY + numTilesY;

        // Keep within the map bounds
        maxX = std::min(maxX, mapWidth - 1);
        maxY = std::min(maxY, mapHeight - 1);

        // Keep within the rendering limits
        maxX = std::min(maxX, RenderUtils::maxTilesX);
        maxY = std::min(maxY, RenderUtils::maxTilesY);

        // Create buffers to hold all our tile data
        int numVertices = tiles.size() * SpriteRenderable::numVerticesPerSprite;
        int positionDataSize = numVertices * SpriteRenderable::numVertexDimensions;
        int texCoordDataSize = numVertices * SpriteRenderable::numTexCoordDimensions;
        std::vector<GLfloat> positions;
        std::vector<GLfloat> texCoords;
        positions.reserve(positionDataSize);
        texCoords.reserve(texCoordDataSize);

        // Add data to buffers
        for (int tileY = minY; tileY <= maxY; ++tileY)
        {

            // Calculate the index of the first tile in this row
            int tileIndex = tileY * mapWidth + minX;

            for (int tileX = minX; tileX <= maxX; ++tileX)
            {

                auto const& tile = tiles[tileIndex];
                const int txIndex = tile.txIndex;

                // Define vertex positions
                //    0------1
                //    | \    |
                //    |   \..|
                //    3----- 2
                float width = static_cast<float>(RenderUtils::tileSpriteWidthPx);
                float height = static_cast<float>(RenderUtils::tileSpriteHeightPx);
                float x1 = static_cast<float>(RenderUtils::tileToPx_X(tileX));
                float y1 = static_cast<float>(RenderUtils::tileToPx_Y(tileX, tileY));
                float x2 = x1 + width;
                float y2 = y1 + height;
                float z = RenderUtils::zTiles;
                std::vector<GLfloat> thisVertexData = { x1, y1, z, x2, y1, z, x2, y2, z, x1, y2, z };

                // Determine texture co-ordinates
                std::vector<GLfloat> thisTexCoords = renderable.spritesheet.getTexCoords(txIndex);

                // Copy this tile's data to the main buffers
                positions.insert(positions.cend(), thisVertexData.cbegin(), thisVertexData.cend());
                texCoords.insert(texCoords.cend(), thisTexCoords.cbegin(), thisTexCoords.cend());

                ++tileIndex;
            }
        }

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(GLfloat), positions.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
        glBufferSubData(GL_ARRAY_BUFFER, 0, texCoords.size() * sizeof(GLfloat), texCoords.data());
    }

}  // namespace Rival
