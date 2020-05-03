#include "pch.h"
#include "BuildingRenderer.h"

#include <gl/glew.h>
#include <map>
#include <vector>

#include "Palette.h"
#include "RenderUtils.h"
#include "Rival.h"
#include "Shaders.h"

namespace Rival {

    BuildingRenderer::BuildingRenderer(
        std::map<BuildingType, Spritesheet>& buildingSprites,
        Texture& paletteTexture):
        buildingSprites(buildingSprites),
        paletteTexture(paletteTexture) {
    }

    SpriteRenderable& BuildingRenderer::getOrCreateRenderable(const Building& building) {
        
        int id = building.getId();

        if (renderables.find(id) == renderables.end()) {
            // No Renderable exists for this Building; create one
            const BuildingType type = building.getType();
            const Spritesheet& sprite = buildingSprites.at(type);
            renderables.emplace(std::piecewise_construct,
                                std::forward_as_tuple(id),
                                std::forward_as_tuple(sprite, 1));
        }

        return renderables.at(id);
    }

    void BuildingRenderer::render(
        const Camera& camera,
        const std::map<int, std::unique_ptr<Building>>& buildings) {

        for (auto const& kv : buildings) {
            const Building& building = *kv.second;

            // If Building is deleted, remove the associated Renderable
            if (building.isDeleted()) {
                renderables.erase(building.getId());
                continue;
            }

            if (isBuildingVisible(building, camera)) {
                renderBuilding(building);
            }
        }

    }

    bool BuildingRenderer::isBuildingVisible(const Building& building, const Camera& camera) {
        // Check if any corner of the unit is visible
        float x1 = static_cast<float>(building.getX());
        float y1 = static_cast<float>(building.getY());
        float x2 = static_cast<float>(building.getX() + Building::width);
        float y2 = static_cast<float>(building.getY() + Building::height);

        return camera.contains(x1, y1)
            || camera.contains(x2, y1)
            || camera.contains(x2, y2)
            || camera.contains(x1, y2);
    }

    void BuildingRenderer::renderBuilding(const Building& building) {

        // Get (or create) the Renderable for this Unit
        SpriteRenderable& renderable = getOrCreateRenderable(building);

        // Determine the frame of the texture to be rendered
        int txIndex = getTxIndex(building);

        // Define vertex positions
        float width = static_cast<float>(RenderUtils::buildingWidthPx);
        float height = static_cast<float>(RenderUtils::buildingHeightPx);
        float x1 = static_cast<float>(
            RenderUtils::tileToPx_X(building.getX()));
        float y1 = static_cast<float>(
            RenderUtils::tileToPx_Y(building.getX(), building.getY()));
        x1 += static_cast<float>(buildingOffsetX);
        y1 += static_cast<float>(buildingOffsetY);
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

    int BuildingRenderer::getTxIndex(const Building& building) const {
        return building.getCurrentSpriteIndex();
    }

}
