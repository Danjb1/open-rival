#include "gfx/renderer/EntityOverlayRenderer.h"

#include "entity/Entity.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/SpriteComponent.h"
#include "gfx/Camera.h"
#include "gfx/RenderUtils.h"
#include "gfx/Shaders.h"
#include "utils/LogUtils.h"

namespace Rival {

EntityOverlayRenderer::EntityOverlayRenderer()
    : boxRenderable(maxBoxesToRender)
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

    // Define vertex positions
    const MapNode& pos = entity.getPos();
    const float x1 = static_cast<float>(RenderUtils::tileToPx_X(pos.x))
            + static_cast<float>(RenderUtils::entityDrawOffsetX)  //
            + spriteComponent->lastLerpOffset.x;
    const float y1 = static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y))
            + static_cast<float>(RenderUtils::entityDrawOffsetY)  //
            + spriteComponent->lastLerpOffset.y;
    const float x2 = x1 + 10.f;  // TMP: width
    const float y2 = y1 + 5.f;   // TMP: height

    std::vector<GLfloat> newVerts = {
        /* clang-format off */
        x1, y1,
        x2, y1,
        x2, y2,
        x1, y2
        /* clang-format on */
    };
    vertexData.insert(vertexData.end(), newVerts.begin(), newVerts.end());
    ++numBoxes;

    const std::vector<GLfloat> newColors = {
        /* clang-format off */
        1.f, 1.f, 1.f, 1.f, // TMP: solid white
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        /* clang-format on */
    };
    colorData.insert(colorData.end(), newColors.begin(), newColors.end());
}

}  // namespace Rival
