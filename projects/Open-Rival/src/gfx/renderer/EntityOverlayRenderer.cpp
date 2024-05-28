#include "gfx/renderer/EntityOverlayRenderer.h"

#include "application/Resources.h"
#include "entity/Entity.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/SpriteComponent.h"
#include "gfx/Camera.h"
#include "gfx/Palette.h"
#include "gfx/PaletteUtils.h"
#include "gfx/Shaders.h"
#include "utils/LogUtils.h"

namespace Rival {

const std::string EntityOverlayRenderer::healthBarAtlasKey = "img_overlay_health.tga";
const std::string EntityOverlayRenderer::healthBarDepletedAtlasKey = "img_overlay_health_depleted.tga";
const std::string EntityOverlayRenderer::monsterHealthBarAtlasKey = "img_overlay_health_monster.tga";
const std::string EntityOverlayRenderer::monsterHealthBarDepletedAtlasKey = "img_overlay_health_monster_depleted.tga";

EntityOverlayRenderer::EntityOverlayRenderer(const TextureStore& textureStore)
    : paletteTexture(textureStore.getPalette())
    , overlayRenderable(textureStore.getOverlayTextureAtlas(), maxImagesToRender)
{
    vertexData.reserve(
            maxImagesToRender * AtlasRenderable::numVerticesPerSprite * AtlasRenderable::numVertexDimensions);
    texCoordData.reserve(
            maxImagesToRender * AtlasRenderable::numVerticesPerSprite * AtlasRenderable::numTexCoordDimensions);
}

void EntityOverlayRenderer::render(const EntityContainer& entityContainer)
{
    // Fill buffers
    vertexData.clear();
    texCoordData.clear();
    int numSprites = 0;
    entityContainer.forEachEntity([&](const auto& entity) { addEntityOverlayToBuffers(*entity, numSprites); });

    // Bind vertex array
    glBindVertexArray(overlayRenderable.getVao());

    // Set palette
    glUniform1f(Shaders::worldShader.paletteTxYUnitUniformLoc, PaletteUtils::paletteIndexGame);

    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, overlayRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, paletteTexture->getId());

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, overlayRenderable.getPositionVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(GLfloat), vertexData.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, overlayRenderable.getTexCoordVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordData.size() * sizeof(GLfloat), texCoordData.data());

    // Render
    const int numIndices = overlayRenderable.getIndicesPerSprite() * numSprites;
    glDrawElements(overlayRenderable.getDrawMode(), numIndices, GL_UNSIGNED_INT, nullptr);
}

void EntityOverlayRenderer::addEntityOverlayToBuffers(const Entity& entity, int& numSprites)
{
    if (numSprites >= maxImagesToRender)
    {
        // Don't overflow the buffer
        return;
    }

    const HealthComponent* healthComponent = entity.getComponent<HealthComponent>(HealthComponent::key);
    if (!healthComponent)
    {
        return;
    }

    const SpriteComponent* spriteComponent = entity.getComponent<SpriteComponent>(SpriteComponent::key);
    if (!spriteComponent)
    {
        return;
    }

    // Define vertex positions
    const MapNode& pos = entity.getPos();
    const float x1 = static_cast<float>(RenderUtils::tileToPx_X(pos.x))  //
            + static_cast<float>(healthBarDrawOffsetX)                   //
            + spriteComponent->lastLerpOffset.x;
    const float y1 = static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y))  //
            + static_cast<float>(healthBarDrawOffsetY)                          //
            + spriteComponent->lastLerpOffset.y;                                //
    const float x2 = x1 + healthBarWidth;
    const float y2 = y1 + healthBarHeight;
    const float z = 0.f;  // Unused

    std::vector<GLfloat> newVerts = {
        /* clang-format off */
        x1, y1, z,
        x2, y1, z,
        x2, y2, z,
        x1, y2, z
        /* clang-format on */
    };
    vertexData.insert(vertexData.end(), newVerts.cbegin(), newVerts.cend());

    // Define texture co-ords
    std::vector<GLfloat> texCoords = overlayRenderable.texAtlas->getTexCoords(healthBarAtlasKey);
    texCoordData.insert(texCoordData.end(), texCoords.cbegin(), texCoords.cend());

    ++numSprites;
}

}  // namespace Rival
