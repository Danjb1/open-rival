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

        // Enable vertex attributes
        glEnableVertexAttribArray(textureShader.vertexAttribLocation);
        glEnableVertexAttribArray(textureShader.texCoordAttribLocation);

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
            float width = static_cast<float>(Sprite::tileWidthPx);
            float height = static_cast<float>(Sprite::tileHeightPx);
            float x = getTileRenderPosX(tileX, tileY);
            float y = getTileRenderPosY(tileY);
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

            // Create vertex position VBO
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
            glBufferData(
                GL_ARRAY_BUFFER,
                2 * 4 * sizeof(GLfloat),
                vertexData.data(),
                GL_DYNAMIC_DRAW);

            // Create tex coord VBO
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
            glBufferData(
                GL_ARRAY_BUFFER,
                2 * 4 * sizeof(GLfloat),
                texCoords.data(),
                GL_DYNAMIC_DRAW);

            // Use vertex position VBO
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());

            // Define vertex position VBO data format
            glVertexAttribPointer(
                textureShader.vertexAttribLocation,
                2,
                GL_FLOAT,
                GL_FALSE,
                2 * sizeof(GLfloat),
                nullptr);

            // Use texture co-ordinate VBO
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());

            // Define texture co-ordinate VBO data format
            glVertexAttribPointer(
                textureShader.texCoordAttribLocation,
                2,
                GL_FLOAT,
                GL_FALSE,
                2 * sizeof(GLfloat),
                nullptr);

            // Bind index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.getIbo());

            // Render
            glDrawElements(
                GL_TRIANGLE_FAN,
                4,
                GL_UNSIGNED_INT,
                nullptr);
        }

        // Clean up
        glDisableVertexAttribArray(textureShader.vertexAttribLocation);
        glDisableVertexAttribArray(textureShader.texCoordAttribLocation);
        glBindTexture(GL_TEXTURE_2D, 0);

        glUseProgram(0);
    }

    float TileRenderer::getTileRenderPosX(int x, int y) {
        if (y % 2 == 0) {
            return static_cast<float>(x) * 64;
        } else {
            return static_cast<float>(x) * 64 + 32;
        }
    }

    float TileRenderer::getTileRenderPosY(int y) {
        return static_cast<float>(y) * 16;
    }

}
