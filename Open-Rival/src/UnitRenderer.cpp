#include "pch.h"
#include "UnitRenderer.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

#include "Palette.h"
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

            // Determine view-projection matrix
            glm::mat4 projection = glm::ortho(
                -3.0f,    // left
                3.0f,     // right
                3.0f,     // bottom
                -3.0f);   // top

            // Right handed rule:
            // x points right
            // y points down
            // z points into the screen
            glm::mat4 view = glm::lookAt(
                glm::vec3(0, 0, -1),    // camera position
                glm::vec3(0, 0, 0),     // look at
                glm::vec3(0, -1, 0)     // up vector
            );
            glm::mat4 viewProjMatrix = projection * view;

            // Define vertex positions
            float x1 = static_cast<float>(unit->getX()) - 0.5f;
            float y1 = static_cast<float>(unit->getY()) - 0.5f;
            float x2 = static_cast<float>(unit->getX()) + 0.5f;
            float y2 = static_cast<float>(unit->getY()) + 0.5f;
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

            // Enable vertex attributes
            glEnableVertexAttribArray(textureShader.vertexAttribLocation);
            glEnableVertexAttribArray(textureShader.texCoordAttribLocation);

            // Set uniform values
            glUniformMatrix4fv(textureShader.viewProjMatrixUniformLocation,
                    1, GL_FALSE, &viewProjMatrix[0][0]);
            glUniform1i(textureShader.texUnitUniformLocation, 0);
            glUniform1i(textureShader.paletteTexUnitUniformLocation, 1);

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

            // Clean up
            glDisableVertexAttribArray(textureShader.vertexAttribLocation);
            glDisableVertexAttribArray(textureShader.texCoordAttribLocation);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glUseProgram(0);
    }

}
