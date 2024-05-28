#include "gfx/renderer/EntityRenderer.h"

#include "gfx/GlewWrapper.h"

#include <algorithm>  // std::min
#include <vector>

#include "application/Resources.h"
#include "entity/Entity.h"
#include "entity/components/MouseHandlerComponent.h"
#include "entity/components/OwnerComponent.h"
#include "entity/components/SpriteComponent.h"
#include "entity/components/WalkerComponent.h"
#include "game/MapUtils.h"
#include "game/Pathfinding.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"
#include "gfx/Camera.h"
#include "gfx/Palette.h"
#include "gfx/RenderUtils.h"
#include "gfx/Shaders.h"
#include "gfx/Spritesheet.h"
#include "gfx/Texture.h"
#include "utils/LogUtils.h"

namespace Rival {

EntityRenderer::EntityRenderer(
        const TextureStore& textureStore, const PlayerContext& playerContext, const PlayerStore& playerStore)
    : paletteTexture(textureStore.getPalette())
    , hitboxRenderable(textureStore.getHitboxSpritesheet(), numHitboxSprites)
    , playerContext(playerContext)
    , playerStore(playerStore)
{
}

void EntityRenderer::render(const Camera& camera, const EntityContainer& entityContainer, int delta) const
{
    entityContainer.forEachEntity([&](const auto& entity) {
        if (isEntityVisible(*entity, camera))
        {
            renderEntity(*entity, delta);
        }
    });

    // Restore the default palette
    glUniform1f(Shaders::worldShader.paletteTxYUnitUniformLoc, 0);
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

void EntityRenderer::renderEntity(const Entity& entity, int delta) const
{
    // Get this Entity's SpriteComponent
    const SpriteComponent* spriteComponent = entity.getComponent<SpriteComponent>(SpriteComponent::key);

    // Entities without a SpriteComponent cannot be rendered
    if (!spriteComponent)
    {
        return;
    }

    // Determine palette based on owner
    int paletteIndex = PaletteUtils::paletteIndexGame;
    const auto owner = entity.getComponent<OwnerComponent>(OwnerComponent::key);
    if (owner)
    {
        paletteIndex += owner->getPlayerId();
    }
    float paletteTxy = PaletteUtils::getPaletteTxY(paletteIndex);
    glUniform1f(Shaders::worldShader.paletteTxYUnitUniformLoc, paletteTxy);

    // Use textures
    const SpriteRenderable& renderable = spriteComponent->getRenderable();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, paletteTexture->getId());

    // Bind vertex array
    glBindVertexArray(renderable.getVao());

    // Update the data on the GPU
    if (needsUpdate(entity, *spriteComponent))
    {
        sendDataToGpu(entity, *spriteComponent, delta);
    }

    // Render
    glDrawElements(renderable.getDrawMode(), renderable.getIndicesPerSprite(), GL_UNSIGNED_INT, nullptr);

    // Render hitbox
    if (isEntityUnderMouse(entity))
    {
        glUniform1f(Shaders::worldShader.paletteTxYUnitUniformLoc, 0);  // Use default palette for hitboxes
        renderHitbox(entity);
    }
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

    glm::vec2 lerpOffset = getLerpOffset(entity, delta);
    spriteComponent.lastLerpOffset = lerpOffset;
    x1 += lerpOffset.x;
    y1 += lerpOffset.y;

    float width = static_cast<float>(RenderUtils::entityWidthPx);
    float height = static_cast<float>(RenderUtils::entityHeightPx);
    float x2 = x1 + width;
    float y2 = y1 + height;

    float z = RenderUtils::getEntityZ(pos.x, pos.y);
    std::vector<GLfloat> vertexData = {
        /* clang-format off */
        x1, y1, z,
        x2, y1, z,
        x2, y2, z,
        x1, y2, z
        /* clang-format on */
    };

    // Determine texture co-ordinates
    const SpriteRenderable& renderable = spriteComponent.getRenderable();
    std::vector<GLfloat> texCoords = renderable.spritesheet.getTexCoords(txIndex);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
    int positionBufferSize = static_cast<int>(vertexData.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, vertexData.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, renderable.getTexCoordVbo());
    int texCoordBufferSize = static_cast<int>(texCoords.size() * sizeof(GLfloat));
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
glm::vec2 EntityRenderer::getLerpOffset(const Entity& entity, int delta)
{
    glm::vec2 offset = { 0, 0 };

    // See if the Entity can move
    const MovementComponent* moveComponent = entity.getComponent<MovementComponent>(MovementComponent::key);
    if (!moveComponent)
    {
        return offset;
    }

    // See if the Entity is currently moving
    const Movement& movement = moveComponent->getMovement();
    if (!movement.isValid())
    {
        return offset;
    }

    // Determine the direction of movement
    const Facing dir = MapUtils::getDir(movement.start, movement.destination);

    // Determine the overall "progress" through the movement (0-1)
    const int timeElapsed = movement.timeElapsed + delta;
    float progress = std::min(static_cast<float>(timeElapsed) / movement.timeRequired, 1.f);

    if (movement.isPastHalfway())
    {
        // Once an Entity has passed the halfway point of its movement, it is considered to be in the new tile.
        // But since progress is measured from the previous tile, we need to visually "undo" this movement otherwise
        // the entity will jump ahead.
        progress -= 1.f;
    }

    // Determine x-offset
    if (dir == Facing::East)
    {
        offset.x = progress * RenderUtils::tileWidthPx;
    }
    else if (dir == Facing::West)
    {
        offset.x = -progress * RenderUtils::tileWidthPx;
    }
    else if (dir == Facing::NorthEast || dir == Facing::SouthEast)
    {
        offset.x = progress * 0.5f * RenderUtils::tileWidthPx;
    }
    else if (dir == Facing::NorthWest || dir == Facing::SouthWest)
    {
        offset.x = -progress * 0.5f * RenderUtils::tileWidthPx;
    }

    // Determine y-offset
    if (dir == Facing::North)
    {
        offset.y = -progress * RenderUtils::tileHeightPx;
    }
    else if (dir == Facing::South)
    {
        offset.y = progress * RenderUtils::tileHeightPx;
    }
    else if (dir == Facing::NorthEast || dir == Facing::NorthWest)
    {
        offset.y = -progress * 0.5f * RenderUtils::tileHeightPx;
    }
    else if (dir == Facing::SouthEast || dir == Facing::SouthWest)
    {
        offset.y = progress * 0.5f * RenderUtils::tileHeightPx;
    }

    return offset;
}

bool EntityRenderer::isEntityUnderMouse(const Entity& entity) const
{
    if (const auto& entityUnderMouse = playerContext.weakEntityUnderMouse.lock())
    {
        return entity.getId() == entityUnderMouse->getId();
    }

    return false;
}

void EntityRenderer::renderHitbox(const Entity& entity) const
{
    const MouseHandlerComponent* mouseHandler = entity.getComponent<MouseHandlerComponent>(MouseHandlerComponent::key);
    if (!mouseHandler)
    {
        LOG_WARN("Could not find MouseHandlerComponent for entity under mouse");
        return;
    }

    const Rect& hitbox = mouseHandler->getHitbox();
    const OwnerComponent* ownerComp = entity.getComponent<OwnerComponent>(OwnerComponent::key);
    const bool isSelf = ownerComp && playerStore.isLocalPlayer(ownerComp->getPlayerId());
    const int baseTexCoord = isSelf ? 0 : 4;  // Use red hitbox for enemies

    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hitboxRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, paletteTexture->getId());

    // Bind vertex array
    glBindVertexArray(hitboxRenderable.getVao());

    // Prepare data for GPU
    std::vector<GLfloat> positions;
    positions.reserve(
            SpriteRenderable::numVerticesPerSprite * SpriteRenderable::numVertexDimensions * numHitboxSprites);
    std::vector<GLfloat> texCoords;
    texCoords.reserve(
            SpriteRenderable::numVerticesPerSprite * SpriteRenderable::numTexCoordDimensions * numHitboxSprites);

    // Top-left corner
    {
        float x1 = hitbox.x;
        float y1 = hitbox.y;
        float x2 = x1 + RenderUtils::hitboxSpriteWidthPx;
        float y2 = y1 + RenderUtils::hitboxSpriteHeightPx;
        float z = RenderUtils::zHitbox;
        std::vector<GLfloat> newPositions = {
            /* clang-format off */
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
            /* clang-format on */
        };
        positions.insert(positions.cend(), newPositions.cbegin(), newPositions.cend());

        std::vector<GLfloat> newTexCoords = hitboxRenderable.spritesheet.getTexCoords(baseTexCoord + 0);
        texCoords.insert(texCoords.cend(), newTexCoords.cbegin(), newTexCoords.cend());
    }

    // Top-right corner
    {
        float x1 = hitbox.x + hitbox.width - RenderUtils::hitboxSpriteWidthPx;
        float y1 = hitbox.y;
        float x2 = x1 + RenderUtils::hitboxSpriteWidthPx;
        float y2 = y1 + RenderUtils::hitboxSpriteHeightPx;
        float z = RenderUtils::zHitbox;
        std::vector<GLfloat> newPositions = {
            /* clang-format off */
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
            /* clang-format on */
        };
        positions.insert(positions.cend(), newPositions.cbegin(), newPositions.cend());

        std::vector<GLfloat> newTexCoords = hitboxRenderable.spritesheet.getTexCoords(baseTexCoord + 1);
        texCoords.insert(texCoords.cend(), newTexCoords.cbegin(), newTexCoords.cend());
    }

    // Bottom-right corner
    {
        float x1 = hitbox.x + hitbox.width - RenderUtils::hitboxSpriteWidthPx;
        float y1 = hitbox.y + hitbox.height - RenderUtils::hitboxSpriteHeightPx;
        float x2 = x1 + RenderUtils::hitboxSpriteWidthPx;
        float y2 = y1 + RenderUtils::hitboxSpriteHeightPx;
        float z = RenderUtils::zHitbox;
        std::vector<GLfloat> newPositions = {
            /* clang-format off */
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
            /* clang-format on */
        };
        positions.insert(positions.cend(), newPositions.cbegin(), newPositions.cend());

        std::vector<GLfloat> newTexCoords = hitboxRenderable.spritesheet.getTexCoords(baseTexCoord + 2);
        texCoords.insert(texCoords.cend(), newTexCoords.cbegin(), newTexCoords.cend());
    }

    // Bottom-left corner
    {
        float x1 = hitbox.x;
        float y1 = hitbox.y + hitbox.height - RenderUtils::hitboxSpriteHeightPx;
        float x2 = x1 + RenderUtils::hitboxSpriteWidthPx;
        float y2 = y1 + RenderUtils::hitboxSpriteHeightPx;
        float z = RenderUtils::zHitbox;
        std::vector<GLfloat> newPositions = {
            /* clang-format off */
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
            /* clang-format on */
        };
        positions.insert(positions.cend(), newPositions.cbegin(), newPositions.cend());

        std::vector<GLfloat> newTexCoords = hitboxRenderable.spritesheet.getTexCoords(baseTexCoord + 3);
        texCoords.insert(texCoords.cend(), newTexCoords.cbegin(), newTexCoords.cend());
    }

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, hitboxRenderable.getPositionVbo());
    int positionBufferSize = static_cast<int>(positions.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, hitboxRenderable.getTexCoordVbo());
    int texCoordBufferSize = static_cast<int>(texCoords.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());

    // Render
    glDrawElements(hitboxRenderable.getDrawMode(),
            hitboxRenderable.getIndicesPerSprite() * numHitboxSprites,
            GL_UNSIGNED_INT,
            nullptr);
}

}  // namespace Rival
