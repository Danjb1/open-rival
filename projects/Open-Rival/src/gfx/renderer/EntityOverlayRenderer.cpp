#include "gfx/renderer/EntityOverlayRenderer.h"

#include "application/Resources.h"
#include "entity/Entity.h"
#include "entity/Unit.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/OwnerComponent.h"
#include "entity/components/SpriteComponent.h"
#include "gfx/Camera.h"
#include "gfx/Palette.h"
#include "gfx/PaletteUtils.h"
#include "gfx/Shaders.h"
#include "utils/LogUtils.h"
#include "utils/MathUtils.h"

namespace Rival {

const std::string EntityOverlayRenderer::healthBarAtlasKey = "img_overlay_health.tga";
const std::string EntityOverlayRenderer::healthBarDepletedAtlasKey = "img_overlay_health_depleted.tga";
const std::string EntityOverlayRenderer::monsterHealthBarAtlasKey = "img_overlay_health_monster.tga";
const std::string EntityOverlayRenderer::monsterHealthBarDepletedAtlasKey = "img_overlay_health_monster_depleted.tga";

EntityOverlayRenderer::EntityOverlayRenderer(const TextureStore& textureStore)
    : paletteTexture(textureStore.getPalette())
    , overlayRenderable(textureStore.getOverlayTextureAtlas(), maxImagesToRender)
    , boxRenderable(maxOverlaysToRender)
    , backgroundColor(PaletteUtils::fromHex(Palette::paletteGame[212]))
{
    atlasVertexData.reserve(
            maxImagesToRender * AtlasRenderable::numVerticesPerSprite * AtlasRenderable::numVertexDimensions);
    atlasTexCoordData.reserve(
            maxImagesToRender * AtlasRenderable::numVerticesPerSprite * AtlasRenderable::numTexCoordDimensions);
}

void EntityOverlayRenderer::prepare(const EntityContainer& entityContainer)
{
    // Reset
    atlasVertexData.clear();
    atlasTexCoordData.clear();
    boxVertexData.clear();
    boxColorData.clear();
    numBoxes = 0;
    numSprites = 0;

    // Fill buffers
    entityContainer.forEachEntity([&](const auto& entity) { addEntityOverlayToBuffers(*entity); });
}

void EntityOverlayRenderer::renderBoxes()
{
    // Bind vertex array
    glBindVertexArray(boxRenderable.getVao());

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, boxRenderable.getPositionVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, boxVertexData.size() * sizeof(GLfloat), boxVertexData.data());

    // Upload color data
    glBindBuffer(GL_ARRAY_BUFFER, boxRenderable.getColorVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, boxColorData.size() * sizeof(GLfloat), boxColorData.data());

    // Render
    const int numIndices = boxRenderable.getIndicesPerBox() * numBoxes;
    glDrawElements(boxRenderable.getDrawMode(), numIndices, GL_UNSIGNED_INT, nullptr);
}

void EntityOverlayRenderer::renderTextures()
{
    // Bind vertex array
    glBindVertexArray(overlayRenderable.getVao());

    // Set palette
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, PaletteUtils::paletteIndexGame);

    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, overlayRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, paletteTexture->getId());

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, overlayRenderable.getPositionVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, atlasVertexData.size() * sizeof(GLfloat), atlasVertexData.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, overlayRenderable.getTexCoordVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, atlasTexCoordData.size() * sizeof(GLfloat), atlasTexCoordData.data());

    // Render
    const int numIndices = overlayRenderable.getIndicesPerSprite() * numSprites;
    glDrawElements(overlayRenderable.getDrawMode(), numIndices, GL_UNSIGNED_INT, nullptr);
}

// TODO: This should be a common utility function
bool isMonster(const Entity& entity)
{
    if (const Unit* unit = entity.as<Unit>())
    {
        // Monsters have no owner
        return unit->getComponent<OwnerComponent>(OwnerComponent::key) == nullptr;
    }

    return false;
}

void EntityOverlayRenderer::addEntityOverlayToBuffers(const Entity& entity)
{
    if (numSprites + maxSpritesPerEntity >= maxImagesToRender || numBoxes + 1 >= maxOverlaysToRender)
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

    const MapNode& pos = entity.getPos();
    const float originX = static_cast<float>(RenderUtils::tileToPx_X(pos.x))  //
            + static_cast<float>(healthBarDrawOffsetX)                        //
            + spriteComponent->lastLerpOffset.x;
    const float originY = static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y))  //
            + static_cast<float>(healthBarDrawOffsetY)                               //
            + spriteComponent->lastLerpOffset.y;                                     //

    // Background
    {
        // Define vertex positions
        const float x1 = originX;
        const float y1 = originY;
        const float x2 = x1 + healthBarWidth;
        const float y2 = y1 + 2 * healthBarHeight;

        std::vector<GLfloat> newVerts = {
            /* clang-format off */
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
            /* clang-format on */
        };
        boxVertexData.insert(boxVertexData.end(), newVerts.cbegin(), newVerts.cend());

        // Define colors
        boxColorData.insert(boxColorData.end(), backgroundColor.cbegin(), backgroundColor.cend());  // top-left
        boxColorData.insert(boxColorData.end(), backgroundColor.cbegin(), backgroundColor.cend());  // top-right
        boxColorData.insert(boxColorData.end(), backgroundColor.cbegin(), backgroundColor.cend());  // bottom-right
        boxColorData.insert(boxColorData.end(), backgroundColor.cbegin(), backgroundColor.cend());  // bottom-left

        ++numBoxes;
    }

    const bool bIsMonster = isMonster(entity);
    const std::string currentHealthBarAtlasKey = bIsMonster ? monsterHealthBarAtlasKey : healthBarAtlasKey;
    const std::string currentDepletedHealthBarAtlasKey =
            bIsMonster ? monsterHealthBarDepletedAtlasKey : healthBarDepletedAtlasKey;

    // Health bar 1
    {
        const float widthRatio =
                MathUtils::clampf(healthComponent->getHealth() / static_cast<float>(healthPerBar), 0.f, 1.f);

        // Define vertex positions
        const float x1 = originX;
        const float y1 = originY;
        const float x2 = x1 + healthBarWidth * widthRatio;
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
        atlasVertexData.insert(atlasVertexData.end(), newVerts.cbegin(), newVerts.cend());

        // Define texture co-ords
        std::vector<GLfloat> texCoords = overlayRenderable.texAtlas->getTexCoords(currentHealthBarAtlasKey);
        atlasTexCoordData.insert(atlasTexCoordData.end(), texCoords.cbegin(), texCoords.cend());

        ++numSprites;
    }

    // Health bar 2
    const bool hasSecondBar = healthComponent->getMaxHealth() > healthPerBar;
    if (hasSecondBar)
    {
        const int healthThisBar = healthComponent->getHealth() - healthPerBar;
        const float widthRatio = MathUtils::clampf(healthThisBar / static_cast<float>(healthPerBar), 0.f, 1.f);

        // Define vertex positions
        const float x1 = originX;
        const float y1 = originY + healthBarHeight;
        const float x2 = x1 + healthBarWidth * widthRatio;
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
        atlasVertexData.insert(atlasVertexData.end(), newVerts.cbegin(), newVerts.cend());

        // Define texture co-ords
        std::vector<GLfloat> texCoords = overlayRenderable.texAtlas->getTexCoords(currentHealthBarAtlasKey);
        atlasTexCoordData.insert(atlasTexCoordData.end(), texCoords.cbegin(), texCoords.cend());

        ++numSprites;
    }
}

}  // namespace Rival
