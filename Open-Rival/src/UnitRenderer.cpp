#include "pch.h"
#include "UnitRenderer.h"

#include <gl/glew.h>
#include <map>
#include <vector>

#include "Palette.h"
#include "RenderUtils.h"
#include "Rival.h"
#include "Shaders.h"

namespace Rival {

    UnitRenderer::UnitRenderer(
            std::map<UnitType, Spritesheet>& unitSprites,
            Texture& paletteTexture) :
        unitSprites(unitSprites),
        paletteTexture(paletteTexture) {}

    SpriteRenderable& UnitRenderer::getOrCreateRenderable(const Unit& unit) {

        int id = unit.getId();

        if (renderables.find(id) == renderables.end()) {
            // No Renderable exists for this Unit; create one
            const UnitType type = unit.getType();
            const Spritesheet& sprite = unitSprites.at(type);
            renderables.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(sprite, 1));
        }

        return renderables.at(id);
    }

    void UnitRenderer::render(
            const Camera& camera,
            const std::map<int, std::unique_ptr<Unit>>& units) {

        for (auto const& kv : units) {
            const Unit& unit = *(kv.second).get();

            // If Unit is deleted, remove the associated Renderable
            if (unit.isDeleted()) {
                renderables.erase(unit.getId());
                continue;
            }

            if (isUnitVisible(unit, camera)) {
                renderUnit(unit);
            }
        }
    }

    bool UnitRenderer::isUnitVisible(const Unit& unit, const Camera& camera) {
        // Check if any corner of the unit is visible
        float x1 = static_cast<float>(unit.getX());
        float y1 = static_cast<float>(unit.getY());
        float x2 = static_cast<float>(unit.getX() + Unit::width);
        float y2 = static_cast<float>(unit.getY() + Unit::height);
        return camera.contains(x1, y1)
                || camera.contains(x2, y1)
                || camera.contains(x2, y2)
                || camera.contains(x1, y2);
    }

    void UnitRenderer::renderUnit(const Unit& unit) {

        // Get (or create) the Renderable for this Unit
        SpriteRenderable& renderable = getOrCreateRenderable(unit);

        // Determine the frame of the texture to be rendered
        int txIndex = getTxIndex(unit);

        // Define vertex positions
        float width = static_cast<float>(RenderUtils::unitWidthPx);
        float height = static_cast<float>(RenderUtils::unitHeightPx);
        float x1 = static_cast<float>(
                RenderUtils::tileToPx_X(unit.getX()));
        float y1 = static_cast<float>(
                RenderUtils::tileToPx_Y(unit.getX(), unit.getY()));
        x1 += static_cast<float>(unitOffsetX);
        y1 += static_cast<float>(unitOffsetY);
        float x2 = x1 + width;
        float y2 = y1 + height;
        std::vector<GLfloat> vertexData = {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        };

        // Determine texture co-ordinates
        std::vector<GLfloat> texCoords =
                renderable.spritesheet.getTexCoords(txIndex);

        // Use textures
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

        // Bind vertex array
        glBindVertexArray(renderable.getVao());

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
        int positionBufferSize =
                vertexData.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                positionBufferSize,
                vertexData.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
        int texCoordBufferSize =
                texCoords.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                texCoordBufferSize,
                texCoords.data());

        // Render
        glDrawElements(
                renderable.getDrawMode(),
                renderable.getIndicesPerSprite(),
                GL_UNSIGNED_INT,
                nullptr);
    }

    int UnitRenderer::getTxIndex(const Unit& unit) const {
        return static_cast<int>(unit.getFacing());
    }

}
