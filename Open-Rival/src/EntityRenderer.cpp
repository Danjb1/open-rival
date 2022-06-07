#include "pch.h"

#include "EntityRenderer.h"

#include <gl/glew.h>

#include <algorithm>  // min
#include <vector>

#include "MapUtils.h"
#include "Palette.h"
#include "Pathfinding.h"
#include "RenderUtils.h"
#include "Shaders.h"
#include "WalkerComponent.h"

namespace Rival {

    EntityRenderer::EntityRenderer(const Texture& paletteTexture)
        : paletteTexture(paletteTexture)
    {
    }

    void
    EntityRenderer::render(const Camera& camera, const std::vector<std::shared_ptr<Entity>> entities, int delta) const
    {
        for (auto const& e : entities)
        {
            if (isEntityVisible(*e, camera))
            {
                renderEntity(*e, delta);
            }
        }
    }

    bool EntityRenderer::isEntityVisible(const Entity& entity, const Camera& camera) const
    {
        // Find the centre of this Entity's tile, in Camera units
        const MapNode& pos = entity.getPos();
        float x = static_cast<float>(pos.x + Camera::tileWidth / 2.0f);
        float y = static_cast<float>(pos.y + Camera::tileHeight / 2.0f);

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
        return camera.contains(x1, y1) || camera.contains(x2, y1) || camera.contains(x2, y2) || camera.contains(x1, y2);
    }

    void EntityRenderer::renderEntity(Entity& entity, int delta) const
    {
        // Get this Entity's SpriteComponent
        const SpriteComponent* spriteComponent = entity.getComponent<SpriteComponent>(SpriteComponent::key);

        // Entities without a SpriteComponent cannot be rendered
        if (!spriteComponent)
        {
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
        if (needsUpdate(entity, *spriteComponent))
        {
            sendDataToGpu(entity, *spriteComponent, delta);
        }

        // Render
        glDrawElements(renderable.getDrawMode(), renderable.getIndicesPerSprite(), GL_UNSIGNED_INT, nullptr);
    }

    bool EntityRenderer::needsUpdate(const Entity& entity, const SpriteComponent& spriteComponent) const
    {
        return entity.moved || spriteComponent.dirty;
    }

    void EntityRenderer::sendDataToGpu(const Entity& entity, const SpriteComponent& spriteComponent, int delta) const
    {

        // Determine the frame of the texture to be rendered
        int txIndex = spriteComponent.getTxIndex();

        // Define vertex positions
        const MapNode& pos = entity.getPos();
        float x1 = static_cast<float>(RenderUtils::tileToPx_X(pos.x));
        float y1 = static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y));
        x1 += static_cast<float>(RenderUtils::entityDrawOffsetX);
        y1 += static_cast<float>(RenderUtils::entityDrawOffsetY);

        std::array<float, numLerpDimensions> lerpOffset = getLerpOffset(entity, delta);
        x1 += lerpOffset[lerpIdxX];
        y1 += lerpOffset[lerpIdxY];

        float width = static_cast<float>(RenderUtils::entityWidthPx);
        float height = static_cast<float>(RenderUtils::entityHeightPx);
        float x2 = x1 + width;
        float y2 = y1 + height;

        float z = RenderUtils::getEntityZ(pos.x, pos.y);
        std::vector<GLfloat> vertexData = {
            x1, y1, z,  //
            x2, y1, z,  //
            x2, y2, z,  //
            x1, y2, z   //
        };

        // Determine texture co-ordinates
        const SpriteRenderable& renderable = spriteComponent.getRenderable();
        std::vector<GLfloat> texCoords = renderable.spritesheet.getTexCoords(txIndex);

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
        int positionBufferSize = vertexData.size() * sizeof(GLfloat);
        glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, vertexData.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
        int texCoordBufferSize = texCoords.size() * sizeof(GLfloat);
        glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());

        // Clear the dirty flag now that the GPU is up to date
        spriteComponent.dirty = false;
    }

    /**
     * Gets the pixel offset to apply to an Entity's position to account for
     * movement between tiles.
     *
     * This takes into account the Entity's current movement and also the amount
     * of time that has elapsed since the last logical update.
     *
     * Due to the way tiles overlap, diagonal moves only actually span half a
     * tile, in terms of pixels.
     */
    std::array<float, EntityRenderer::numLerpDimensions>
    EntityRenderer::getLerpOffset(const Entity& entity, int delta) const
    {
        std::array<float, numLerpDimensions> offset = { 0, 0 };

        // See if the Entity can move
        // TODO: This needs to work for flying units too
        const WalkerComponent* walkerComponent = entity.getComponent<WalkerComponent>(WalkerComponent::key);
        if (!walkerComponent)
        {
            return offset;
        }

        // See if the Entity is currently moving
        const Movement& movement = walkerComponent->getMovement();
        if (movement.timeElapsed == 0)
        {
            return offset;
        }

        // Find the MapNode the Entity is moving to
        const Pathfinding::Route route = walkerComponent->getRoute();
        const MapNode* nextNode = route.peek();
        if (!nextNode)
        {
            // Should never happen since we've already established the Entity
            // is moving!
            return offset;
        }

        // Determine the direction of movement
        Facing dir = MapUtils::getDir(entity.getPos(), *nextNode);

        // Determine the overall "progress" through the movement (0-1)
        int timeElapsed = movement.timeElapsed + delta;
        float progress = std::min(static_cast<float>(timeElapsed) / movement.timeRequired, 1.f);

        // Determine x-offset
        if (dir == Facing::East)
        {
            offset[lerpIdxX] = progress * RenderUtils::tileWidthPx;
        }
        else if (dir == Facing::West)
        {
            offset[lerpIdxX] = -progress * RenderUtils::tileWidthPx;
        }
        else if (dir == Facing::NorthEast || dir == Facing::SouthEast)
        {
            offset[lerpIdxX] = progress * 0.5f * RenderUtils::tileWidthPx;
        }
        else if (dir == Facing::NorthWest || dir == Facing::SouthWest)
        {
            offset[lerpIdxX] = -progress * 0.5f * RenderUtils::tileWidthPx;
        }

        // Determine y-offset
        if (dir == Facing::North)
        {
            offset[lerpIdxY] = -progress * RenderUtils::tileHeightPx;
        }
        else if (dir == Facing::South)
        {
            offset[lerpIdxY] = progress * RenderUtils::tileHeightPx;
        }
        else if (dir == Facing::NorthEast || dir == Facing::NorthWest)
        {
            offset[lerpIdxY] = -progress * 0.5f * RenderUtils::tileHeightPx;
        }
        else if (dir == Facing::SouthEast || dir == Facing::SouthWest)
        {
            offset[lerpIdxY] = progress * 0.5f * RenderUtils::tileHeightPx;
        }

        return offset;
    }

}  // namespace Rival
