#include "pch.h"
#include "UnitRenderer.h"

#include <map>

#include "Palette.h"
#include "Shaders.h"

namespace Rival {

    UnitRenderer::UnitRenderer(Texture& paletteTexture) :
        paletteTexture(paletteTexture) {}

    Renderable& UnitRenderer::getOrCreateRenderable(
                const std::unique_ptr<Unit>& unit) {

        int id = unit->getId();

        if (renderables.find(id) == renderables.end()) {
            // No Renderable exists for this Unit; create one
            const Sprite& sprite = unit->getSprite();
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

            // Define vertex positions
            std::vector<GLfloat> vertexData = {
                -0.5f, -0.5f,
                0.5f, -0.5f,
                0.5f,  0.5f,
                -0.5f,  0.5f
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

            // Prepare
            glEnableVertexAttribArray(textureShader.vertexAttribLocation);
            glEnableVertexAttribArray(textureShader.texCoordAttribLocation);
            glUniform1i(textureShader.texUnitUniformLocation, 0);
            glUniform1i(textureShader.paletteTexUnitUniformLocation, 1);
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
            glVertexAttribPointer(
                textureShader.vertexAttribLocation,
                2,
                GL_FLOAT,
                GL_FALSE,
                2 * sizeof(GLfloat),
                nullptr);
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
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
