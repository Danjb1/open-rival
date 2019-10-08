#include "pch.h"
#include "TileRenderer.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

#include "Palette.h"
#include "Rival.h"
#include "Shaders.h"

namespace Rival {

    const int verticesPerTile = 4;
    const int indicesPerTile = 6; // 2 triangles

    TileRenderer::TileRenderer(
            std::map<int, Sprite>& tileSprites,
            Texture& paletteTexture) :
        tileSprites(tileSprites),
        paletteTexture(paletteTexture) {}

    void TileRenderer::render(
            glm::mat4 viewProjMatrix,
            std::vector<Tile>& tiles,
            int mapWidth,
            int mapHeight,
            bool wilderness) {

        // Pick which Sprite to use
        const Sprite& sprite = wilderness
            ? tileSprites.at(1)
            : tileSprites.at(0);

        // Use shader
        glUseProgram(textureShader.programId);

        // Use textures
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, sprite.texture.getId());
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

        // Set uniform values
        glUniformMatrix4fv(textureShader.viewProjMatrixUniformLocation,
            1, GL_FALSE, &viewProjMatrix[0][0]);
        glUniform1i(textureShader.texUnitUniformLocation, 0);
        glUniform1i(textureShader.paletteTexUnitUniformLocation, 1);

        // Create buffers to hold all our tile data
        int numTiles = mapWidth * mapHeight;
        int numVertices = numTiles * verticesPerTile;
        int vertexDataSize = numVertices * Renderable::numVertexDimensions;
        int texCoordDataSize = numVertices * Renderable::numTexCoordDimensions;
        int indexDataSize = numTiles * indicesPerTile;
        std::vector<GLfloat> vertexData;
        std::vector<GLfloat> texCoords;
        std::vector<GLuint> indexData;
        vertexData.reserve(vertexDataSize);
        texCoords.reserve(texCoordDataSize);
        indexData.reserve(indexDataSize);

        // Add data to buffers
        unsigned int i = 0;
        for (Tile& tile : tiles) {

            const int txIndex = tiles[i].txIndex;
            int tileX = i % mapWidth;
            int tileY = i / mapWidth;

            // Define vertex positions
            //    0------1
            //    | \    |
            //    |   \..|
            //    3----- 2
            float width = static_cast<float>(Sprite::tileSpriteWidthPx);
            float height = static_cast<float>(Sprite::tileSpriteHeightPx);
            float x = static_cast<float>(getTileRenderPosX(tileX));
            float y = static_cast<float>(getTileRenderPosY(tileX, tileY));
            float x1 = x;
            float y1 = y;
            float x2 = x + width;
            float y2 = y + height;
            std::vector<GLfloat> thisVertexData = {
                x1, y1,
                x2, y1,
                x2, y2,
                x1, y2
            };

            // Determine texture co-ordinates
            std::vector<GLfloat> thisTexCoords = sprite.getTexCoords(txIndex);

            // Determine index data
            // First triangle: 0-1-2
            // Second triangle: 2-3-0
            unsigned int startIndex = verticesPerTile * i;
            std::vector<GLuint> thisIndexData = {
                startIndex,
                startIndex + 1,
                startIndex + 2,
                startIndex + 2,
                startIndex + 3,
                startIndex
            };

            // Copy this tile's data to the main buffers
            vertexData.insert(
                vertexData.end(),
                thisVertexData.begin(),
                thisVertexData.end());
            texCoords.insert(
                texCoords.end(),
                thisTexCoords.begin(),
                thisTexCoords.end());
            indexData.insert(
                indexData.end(),
                thisIndexData.begin(),
                thisIndexData.end());

            i++;
        }

        // Bind vertex array
        glBindVertexArray(renderable.getVao());

        // Send position data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
        glBufferData(
            GL_ARRAY_BUFFER,
            vertexDataSize * sizeof(GLfloat),
            vertexData.data(),
            GL_DYNAMIC_DRAW);

        // Send tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
        glBufferData(
            GL_ARRAY_BUFFER,
            texCoordDataSize * sizeof(GLfloat),
            texCoords.data(),
            GL_DYNAMIC_DRAW);

        // Send index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.getIbo());
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indexDataSize * sizeof(GLuint),
            indexData.data(),
            GL_STATIC_DRAW);

        // Render
        glDrawElements(
            GL_TRIANGLES,
            indexDataSize,
            GL_UNSIGNED_INT,
            nullptr);

        // Clean up
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

    int TileRenderer::getTileRenderPosX(int x) {
        // Tiles co-ordinates are consistent in the x direction
        return x * (Sprite::tileWidthPx / 2);
    }

    int TileRenderer::getTileRenderPosY(int x, int y) {
        // Tiles co-ordinates zigzag up and down in the y direction
        int renderPos = y * Sprite::tileHeightPx;
        if (x % 2 == 1) {
            renderPos += Sprite::tileHeightPx / 2;
        }
        return renderPos;
    }

}
