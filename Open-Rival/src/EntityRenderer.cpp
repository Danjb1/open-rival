#include "pch.h"
#include "EntityRenderer.h"

#include <gl/glew.h>
#include <vector>

#include "Palette.h"
#include "RenderUtils.h"
#include "Shaders.h"

namespace Rival {

    EntityRenderer::EntityRenderer(const Texture& paletteTexture)
        : paletteTexture(paletteTexture) {}

    void EntityRenderer::render(
            const Camera& camera,
            const std::vector<std::shared_ptr<Entity>> entities) const {
        for (auto const& e : entities) {
            if (isEntityVisible(*e, camera)) {
                renderEntity(*e);
            }
        }
    }

    bool EntityRenderer::isEntityVisible(
            const Entity& entity,
            const Camera& camera) const {

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

    void EntityRenderer::renderEntity(Entity& entity) const {

        // Get this Entity's SpriteComponent
        SpriteComponent* spriteComponent =
                entity.getComponent<SpriteComponent>(SpriteComponent::key);

        // Entities without a SpriteComponent cannot be rendered
        if (spriteComponent == nullptr) {
            return;
        }

        // Use textures
        const SpriteRenderable& renderable = spriteComponent->getRenderable();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, paletteTexture.getId());

        // Bind vertex array
        glBindVertexArray(renderable.getVao());

        // Update the data on the GPU
        if (needsUpdate(entity, spriteComponent)) {
            sendDataToGpu(entity, spriteComponent);
        }

        // Render
        glDrawElements(
                renderable.getDrawMode(),
                renderable.getIndicesPerSprite(),
                GL_UNSIGNED_INT,
                nullptr);
    }

    bool EntityRenderer::needsUpdate(
            const Entity& entity,
            const SpriteComponent* spriteComponent) const {
        return entity.moved || spriteComponent->dirty;
    }

    void EntityRenderer::sendDataToGpu(
            Entity& entity,
            SpriteComponent* spriteComponent) const {

        // Determine the frame of the texture to be rendered
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
        const SpriteRenderable& renderable = spriteComponent->getRenderable();
        std::vector<GLfloat> texCoords =
                renderable.spritesheet.getTexCoords(txIndex);

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

        // Clear the dirty flag now that the GPU is up to date
        spriteComponent->dirty = false;
    }

}  // namespace Rival
