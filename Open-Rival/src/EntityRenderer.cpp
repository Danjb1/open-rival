#include "pch.h"
#include "EntityRenderer.h"

#include <gl/glew.h>
#include <vector>

#include "Palette.h"
#include "RenderUtils.h"
#include "Shaders.h"
#include "SpriteComponent.h"

namespace Rival {

    EntityRenderer::EntityRenderer(const Texture& paletteTexture)
        : paletteTexture(paletteTexture) {}

    void EntityRenderer::render(
            const Camera& camera,
            const std::map<int, std::unique_ptr<Entity>>& entities) {
        for (auto const& kv : entities) {
            Entity& e = *(kv.second);
            if (isEntityVisible(e, camera)) {
                renderEntity(e);
            }
        }
    }

    bool EntityRenderer::isEntityVisible(
            const Entity& entity,
            const Camera& camera) {

        // Find the centre of this Entity's tile, in Camera units
        float x = static_cast<float>(entity.getX() + Camera::tileWidth / 2.0f);
        float y = static_cast<float>(entity.getY() + Camera::tileHeight / 2.0f);

        // Add some tolerance in all directions to approximate the area where
        // the Entity would be rendered. This is quicker and simpler than
        // trying to calculate the precise rendered position of the Entity,
        // but it may result in some Entities being rendered offscreen.
        float width = RenderUtils::pxToCamera_X(RenderUtils::entityWidthPx);
        float height = RenderUtils::pxToCamera_Y(RenderUtils::entityHeightPx);
        float x1 = x - width / 2.0f;
        float y1 = y - height / 2.0f;
        float x2 = x + width / 2.0f;
        float y2 = y + height / 2.0f;

        // Check if any corner of this area is visible
        return camera.contains(x1, y1)
                || camera.contains(x2, y1)
                || camera.contains(x2, y2)
                || camera.contains(x1, y2);
    }

    void EntityRenderer::renderEntity(const Entity& entity) {

        // Get this Entity's SpriteComponent
        const SpriteComponent* spriteComponent =
                entity.getComponent<SpriteComponent>(SpriteComponent::key);

        // Entities without a SpriteComponent cannot be rendered
        if (spriteComponent == nullptr) {
            return;
        }

        // Determine the frame of the texture to be rendered
        const SpriteRenderable& renderable = spriteComponent->getRenderable();
        int txIndex = spriteComponent->getTxIndex();

        // Define vertex positions
        float width = static_cast<float>(RenderUtils::entityWidthPx);
        float height = static_cast<float>(RenderUtils::entityHeightPx);
        float x1 = static_cast<float>(
                RenderUtils::tileToPx_X(entity.getX()));
        float y1 = static_cast<float>(
                RenderUtils::tileToPx_Y(entity.getX(), entity.getY()));
        x1 += static_cast<float>(RenderUtils::entityDrawOffsetX);
        y1 += static_cast<float>(RenderUtils::entityDrawOffsetY);
        float x2 = x1 + width;
        float y2 = y1 + height;
        float z = RenderUtils::getEntityZ(entity.getX(), entity.getY());
        std::vector<GLfloat> vertexData = {
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
        };

        // Determine texture co-ordinates
        std::vector<GLfloat> texCoords =
                renderable.spritesheet.getTexCoords(txIndex);

        // Use textures
        glActiveTexture(GL_TEXTURE0 + 0);  // Texture entity 0
        glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
        glActiveTexture(GL_TEXTURE0 + 1);  // Texture entity 1
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

}  // namespace Rival
