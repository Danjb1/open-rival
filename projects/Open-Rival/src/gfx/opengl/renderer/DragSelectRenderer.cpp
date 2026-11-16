#include "gfx/opengl/renderer/DragSelectRenderer.h"

#include <glm/vec4.hpp>

#include <algorithm>  // std::min, std::max

#include "game/PlayerContext.h"
#include "gfx/RenderUtils.h"
#include "gfx/opengl/Framebuffer.h"
#include "gfx/opengl/GLUtils.h"
#include "gfx/opengl/Shaders.h"
#include "gfx/opengl/renderable/BoxRenderable.h"

namespace Rival {

DragSelectRenderer::DragSelectRenderer(const PlayerContext& playerContext)
    : playerContext(playerContext)
    , boxRenderable(numBoxes)
{
}

void DragSelectRenderer::render()
{
    // TMP
    const float thickness = 1.f;
    const glm::vec4 color { 1.f, 1.f, 1.f, 1.f };

    // Normalize the drag-select area
    const auto& dragSelect = playerContext.dragSelect;
    const float x1 = static_cast<float>(std::min(dragSelect.startX, dragSelect.endX));
    const float y1 = static_cast<float>(std::min(dragSelect.startY, dragSelect.endY));
    const float x2 = static_cast<float>(std::max(dragSelect.startX, dragSelect.endX));
    const float y2 = static_cast<float>(std::max(dragSelect.startY, dragSelect.endY));

    boxRenderable.setBoxes({
            { { x1, y1 }, { x2, y1 + thickness }, color },  // top
            { { x2, y1 }, { x2 + thickness, y2 }, color },  // right
            { { x1, y2 }, { x2, y2 + thickness }, color },  // bottom
            { { x1, y1 }, { x1 + thickness, y2 }, color },  // left
    });
    boxRenderable.draw();
}

}  // namespace Rival
