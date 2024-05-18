#include "gfx/renderer/TileRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)
#include <algorithm>
#include <map>

#include "gfx/Camera.h"
#include "gfx/Palette.h"
#include "gfx/Shaders.h"
#include "gfx/Spritesheet.h"
#include "gfx/Texture.h"
#include "utils/MathUtils.h"

namespace Rival {

const std::array<int, 4> TileRenderer::waterFrames1 = { 3, 2, 1, 0 };
const std::array<int, 7> TileRenderer::waterFrames2 = { 6, 5, 4, 3, 2, 1, 0 };

TileRenderer::TileRenderer(const Spritesheet& spritesheet, std::shared_ptr<const Texture> paletteTexture)
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
void TileRenderer::render(int delta, const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight)
{
    updateWaterPalettes(delta);

    // Use textures
    glActiveTexture(GL_TEXTURE0 + 0);  // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
    glActiveTexture(GL_TEXTURE0 + 1);  // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, paletteTexture->getId());

    // Bind vertex array
    glBindVertexArray(renderable.getVao());

    // Set uniforms
    glUniform1i(Shaders::worldShader.waterShift1, waterFrames1[waterIndex1]);
    glUniform1i(Shaders::worldShader.waterShift2, waterFrames2[waterIndex2]);

    // Update the data on the GPU
    if (needsUpdate())
    {
        sendDataToGpu(camera, tiles, mapWidth, mapHeight);
    }

    // Render
    int numIndices = static_cast<int>(tiles.size() * renderable.getIndicesPerSprite());
    glDrawElements(renderable.getDrawMode(), numIndices, GL_UNSIGNED_INT, nullptr);
}

void TileRenderer::updateWaterPalettes(int delta)
{
    // Water is animated by rotating a few groups of colors in the palette.
    // We simulate this in the shader by adjusting the palette index based on some offsets that we control.
    msSinceWaterShift += delta;
    if (msSinceWaterShift > msPerWaterFrame)
    {
        waterIndex1 = (waterIndex1 + 1) % waterFrames1.size();
        waterIndex2 = (waterIndex2 + 1) % waterFrames2.size();
        msSinceWaterShift -= msPerWaterFrame;
    }
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

void TileRenderer::sendDataToGpu(
        const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight) const
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
    int numVertices = static_cast<int>(tiles.size() * SpriteRenderable::numVerticesPerSprite);
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

            const auto& tile = tiles[tileIndex];
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
            std::vector<GLfloat> thisVertexData = {
                /* clang-format off */
                x1, y1, z,
                x2, y1, z,
                x2, y2, z,
                x1, y2, z
                /* clang-format on */
            };

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
