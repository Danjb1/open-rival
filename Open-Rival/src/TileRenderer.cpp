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

    TileRenderer::TileRenderer(
            std::map<int, Sprite>& tileSprites,
            Texture& paletteTexture) :
        tileSprites(tileSprites),
        paletteTexture(paletteTexture) {}

    void TileRenderer::render(
            std::vector<int>& tiles,
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

        // Determine view matrix
        glm::mat4 view = glm::lookAt(
            glm::vec3(0, 0, 1),     // camera position
            glm::vec3(0, 0, 0),     // look at
            glm::vec3(0, 1, 0)      // up vector
        );

        // Determine projection matrix
        float screenWidth = static_cast<float>(Rival::windowWidth);
        float screenHeight = static_cast<float>(Rival::windowHeight);
        glm::mat4 projection = glm::ortho(
            0.0f,           // left
            screenWidth,    // right
            screenHeight,   // bottom
            0.0f);          // top

        // Combine matrices
        glm::mat4 viewProjMatrix = projection * view;

        // Set uniform values
        glUniformMatrix4fv(textureShader.viewProjMatrixUniformLocation,
            1, GL_FALSE, &viewProjMatrix[0][0]);
        glUniform1i(textureShader.texUnitUniformLocation, 0);
        glUniform1i(textureShader.paletteTexUnitUniformLocation, 1);

        int i = 0;
        for (int& tile : tiles) {

            const int tile = tiles[i];
            int tileX = i % mapWidth;
            int tileY = i / mapWidth;
            i++;

            // Get the Renderable for this Tile
            Renderable renderable(sprite);

            // Set the txIndex as appropriate
            renderable.setTxIndex(tile);

            // Define vertex positions
            float width = static_cast<float>(Sprite::tileSpriteWidthPx);
            float height = static_cast<float>(Sprite::tileSpriteHeightPx);
            float x = static_cast<float>(getTileRenderPosX(tileX));
            float y = static_cast<float>(getTileRenderPosY(tileX, tileY));
            float x1 = x;
            float y1 = y;
            float x2 = x + width;
            float y2 = y + height;
            std::vector<GLfloat> vertexData = {
                x1, y1,
                x2, y1,
                x2, y2,
                x1, y2
            };

            // Determine texture co-ordinates
            std::vector<GLfloat> texCoords = renderable.getTexCoords();

            // TODO:
            // Build a large array of vertices positions, texture co-ordinates
            // and indices, and move the below code outside the loop using
            // GL_TRIANGLES

            // Bind vertex array
            glBindVertexArray(renderable.getVao());

            // Enable vertex attributes
            glEnableVertexAttribArray(textureShader.vertexAttribIndex);
            glEnableVertexAttribArray(textureShader.texCoordAttribIndex);

            // Send position data
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
            glBufferData(
                GL_ARRAY_BUFFER,
                2 * 4 * sizeof(GLfloat),
                vertexData.data(),
                GL_DYNAMIC_DRAW);

            // Send tex co-ord data
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
            glBufferData(
                GL_ARRAY_BUFFER,
                2 * 4 * sizeof(GLfloat),
                texCoords.data(),
                GL_DYNAMIC_DRAW);

            // Send index data
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.getIbo());

            // Render
            glDrawElements(
                GL_TRIANGLE_FAN,
                4,
                GL_UNSIGNED_INT,
                nullptr);

            // Disable vertex attributes
            glDisableVertexAttribArray(textureShader.vertexAttribIndex);
            glDisableVertexAttribArray(textureShader.texCoordAttribIndex);
        }

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
