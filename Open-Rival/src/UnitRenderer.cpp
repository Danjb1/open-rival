#include "pch.h"
#include "UnitRenderer.h"

#include <map>

#include "Palette.h"
#include "RenderUtils.h"
#include "Rival.h"
#include "Shaders.h"

namespace Rival {

    UnitRenderer::UnitRenderer(
            std::map<Unit::Type, Sprite>& unitSprites,
            Texture& paletteTexture) :
        unitSprites(unitSprites),
        paletteTexture(paletteTexture) {}

    ImageRenderable& UnitRenderer::getOrCreateRenderable(
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

    void UnitRenderer::render(
            glm::mat4 viewProjMatrix,
            std::map<int, std::unique_ptr<Unit>>& units) {

        // Use shader
        glUseProgram(textureShader.programId);

        // Set uniform values
        glUniformMatrix4fv(textureShader.viewProjMatrixUniformLocation,
            1, GL_FALSE, &viewProjMatrix[0][0]);
        glUniform1i(textureShader.texUnitUniformLocation, 0);
        glUniform1i(textureShader.paletteTexUnitUniformLocation, 1);

        for (auto const& kv : units) {
            const std::unique_ptr<Unit>& unit = kv.second;

            // If Unit is deleted, remove the associated Renderable
            if (unit->isDeleted()) {
                renderables.erase(unit->getId());
                continue;
            }

            // Get (or create) the Renderable for this Unit
            ImageRenderable& renderable = getOrCreateRenderable(unit);

            // Set the txIndex as appropriate
            renderable.setTxIndex(unit->getFacing());

            // Define vertex positions
            float width = static_cast<float>(Sprite::unitWidthPx);
            float height = static_cast<float>(Sprite::unitHeightPx);
            float x1 = static_cast<float>(
                    RenderUtils::getRenderPosX(unit->getX()));
            float y1 = static_cast<float>(
                    RenderUtils::getRenderPosY(unit->getX(), unit->getY()));
            float x2 = x1 + width;
            float y2 = y1 + height;
            std::vector<GLfloat> vertexData = {
                x1, y1,
                x2, y1,
                x2, y2,
                x1, y2
            };

            // Determine texture co-ordinates
            std::vector<GLfloat> texCoords = renderable.getTexCoords();

            // Use textures
            glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
            glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
            glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

            // Bind vertex array
            glBindVertexArray(renderable.getVao());

            // Use position data
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
            glBufferData(
                GL_ARRAY_BUFFER,
                2 * 4 * sizeof(GLfloat),
                vertexData.data(),
                GL_DYNAMIC_DRAW);

            // Use tex co-ord data
            glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
            glBufferData(
                GL_ARRAY_BUFFER,
                2 * 4 * sizeof(GLfloat),
                texCoords.data(),
                GL_DYNAMIC_DRAW);

            // Render
            glDrawElements(
                GL_TRIANGLE_FAN,
                4,
                GL_UNSIGNED_INT,
                nullptr);
        }

        // Clean up
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

}
