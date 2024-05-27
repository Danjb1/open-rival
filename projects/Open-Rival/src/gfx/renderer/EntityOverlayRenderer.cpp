#include "gfx/renderer/EntityOverlayRenderer.h"

#include "entity/Entity.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/SpriteComponent.h"
#include "gfx/Camera.h"
#include "gfx/Palette.h"
#include "gfx/PaletteUtils.h"
#include "gfx/Shaders.h"
#include "utils/LogUtils.h"

namespace Rival {

EntityOverlayRenderer::EntityOverlayRenderer()
    : boxRenderable(maxBoxesToRender)
    , healthColor1(PaletteUtils::fromHex(Palette::paletteGame[healthColorIndex1]))
    , healthColor2(PaletteUtils::fromHex(Palette::paletteGame[healthColorIndex2]))
{
    vertexData.reserve(maxBoxesToRender * BoxRenderable::numVerticesPerBox * BoxRenderable::numVertexDimensions);
    colorData.reserve(maxBoxesToRender * BoxRenderable::numVerticesPerBox * BoxRenderable::numColorDimensions);
}

void EntityOverlayRenderer::render(const EntityContainer& entityContainer)
{
    // Fill buffers
    vertexData.clear();
    colorData.clear();
    int numBoxes = 0;
    entityContainer.forEachEntity([&](const auto& entity) { addEntityOverlayToBuffers(*entity, numBoxes); });

    // Bind vertex array
    glBindVertexArray(boxRenderable.getVao());

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, boxRenderable.getPositionVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(GLfloat), vertexData.data());

    // Upload color data
    glBindBuffer(GL_ARRAY_BUFFER, boxRenderable.getColorVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, colorData.size() * sizeof(GLfloat), colorData.data());

    // Render
    const int numIndices = boxRenderable.getIndicesPerBox() * numBoxes;
    glDrawElements(boxRenderable.getDrawMode(), numIndices, GL_UNSIGNED_INT, nullptr);
}

void EntityOverlayRenderer::addEntityOverlayToBuffers(const Entity& entity, int& numBoxes)
{
    if (numBoxes >= maxBoxesToRender)
    {
        // Avoid a buffer overflow
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

    // OpenGL determines colors at pixel centres, so our vertices need to be perfectly aligned with pixel centres in
    // order for the correct colors to be sampled.
    const float halfPixelCorrection = 0.5f;

    // Define vertex positions
    const MapNode& pos = entity.getPos();
    const float x1 = static_cast<float>(RenderUtils::tileToPx_X(pos.x))  //
            + static_cast<float>(healthBarDrawOffsetX)                   //
            + spriteComponent->lastLerpOffset.x;
    float y1 = static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y))  //
            + static_cast<float>(healthBarDrawOffsetY)                    //
            + spriteComponent->lastLerpOffset.y                           //
            + halfPixelCorrection;
    const float x2 = x1 + healthBarWidth;
    float y2 = y1 + healthBarHeight;

    LOG_WARN("Rendering health bar from {} to {}", y1, y2);

    std::vector<GLfloat> newVerts = {
        /* clang-format off */
        x1, y1,
        x2, y1,
        x2, y2,
        x1, y2
        /* clang-format on */
    };
    vertexData.insert(vertexData.end(), newVerts.cbegin(), newVerts.cend());
    ++numBoxes;

    // Define colors.
    // Here we are relying on OpenGL to create a gradient, so we might not end up with *exactly* the same colors as the
    // original game, but literally no-one is going to notice.
    //   TODO: Units with lots of health show multiple bars with a grey box behind.
    //   TODO: Health turns red when depleted.
    //   TODO: Monsters have a silver health bar that turns purple when depleted.
    colorData.insert(colorData.end(), healthColor1.cbegin(), healthColor1.cend());  // top-left
    colorData.insert(colorData.end(), healthColor1.cbegin(), healthColor1.cend());  // top-right
    colorData.insert(colorData.end(), healthColor2.cbegin(), healthColor2.cend());  // bottom-right
    colorData.insert(colorData.end(), healthColor2.cbegin(), healthColor2.cend());  // bottom-left
}

}  // namespace Rival
