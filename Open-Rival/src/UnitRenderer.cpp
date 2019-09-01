#include "pch.h"
#include "UnitRenderer.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

#include "Palette.h"
#include "Rival.h"
#include "Shaders.h"

namespace Rival {

    UnitRenderer::UnitRenderer(
            std::map<Unit::Type, Sprite>& unitSprites,
            Texture& paletteTexture) :
        unitSprites(unitSprites),
        paletteTexture(paletteTexture) {}

    Renderable& UnitRenderer::getOrCreateRenderable(
                const std::unique_ptr<Unit>& unit) {

        int id = unit->getId();

        if (renderables.find(id) == renderables.end()) {
            // No Renderable exists for this Unit; create one
            const Unit::Type type = unit->getType();
            const Sprite& sprite = unitSprites.at(type);
            renderables.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(sprite));
        }

        return renderables.at(id);
    }

    void UnitRenderer::render(std::map<int, std::unique_ptr<Unit>>& units) {

        // Use shader
        glUseProgram(textureShader.programId);

        // Determine view matrix
        // OpenGL uses right handed rule:
        //  - x points right
        //  - y points up
        //  - z points out of the screen
        glm::mat4 view = glm::lookAt(
            glm::vec3(0, 0, 1),     // camera position
            glm::vec3(0, 0, 0),     // look at
            glm::vec3(0, 1, 0)      // up vector
        );

        // Determine projection matrix
        // We want y to point down, so the top and bottom arguments are
        // flipped
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

        for (auto const& kv : units) {
            const std::unique_ptr<Unit>& unit = kv.second;

            // If Unit is deleted, remove the associated Renderable
            if (unit->isDeleted()) {
                renderables.erase(unit->getId());
                continue;
            }

            // Get (or create) the Renderable for this Unit
            Renderable& renderable = getOrCreateRenderable(unit);

            // Set the txIndex as appropriate
            renderable.setTxIndex(unit->getFacing());

            // Define vertex positions
            float width = static_cast<float>(Sprite::unitWidthPx);
            float height = static_cast<float>(Sprite::unitHeightPx);
            float x = getUnitRenderPosX(*unit.get());
            float y = getUnitRenderPosY(*unit.get());
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

            // Use textures
            glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
            glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
            glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

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

    float UnitRenderer::getUnitRenderPosX(Unit& unit) {
        return static_cast<float>(unit.getX()) * 64;
    }

    float UnitRenderer::getUnitRenderPosY(Unit& unit) {
        return static_cast<float>(unit.getY()) * 64;
    }

}
