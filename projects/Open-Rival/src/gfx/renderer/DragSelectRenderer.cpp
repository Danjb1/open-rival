#include "gfx/renderer/DragSelectRenderer.h"

#include "gfx/GlewWrapper.h"

#include <vector>

#include "game/PlayerContext.h"
#include "gfx/Framebuffer.h"
#include "gfx/RenderUtils.h"
#include "gfx/Shaders.h"
#include "gfx/renderable/BoxRenderable.h"

namespace Rival {

DragSelectRenderer::DragSelectRenderer(const PlayerContext& playerContext)
    : playerContext(playerContext)
    , boxRenderable(numBoxes)
{
}

void DragSelectRenderer::render()
{
    // Bind vertex array
    glBindVertexArray(boxRenderable.getVao());

    // TMP
    const float thickness = 1.f;

    // Normalize the drag-select area
    const auto& dragSelect = playerContext.dragSelect;
    int startX = std::min(dragSelect.startX, dragSelect.endX);
    int startY = std::min(dragSelect.startY, dragSelect.endY);
    int endX = std::max(dragSelect.startX, dragSelect.endX);
    int endY = std::max(dragSelect.startY, dragSelect.endY);

    // Define vertex positions
    const float x1 = static_cast<float>(startX);
    const float y1 = static_cast<float>(startY);
    const float x2 = static_cast<float>(endX);
    const float y2 = static_cast<float>(endY);

    /* clang-format off */
    std::vector<GLfloat> vertexData = {
        // Box 1 (top)
        x1, y1,
        x2, y1,
        x2, y1 + thickness,
        x1, y1 + thickness,

        // Box 2 (right)
        x2, y1,
        x2 + thickness, y1,
        x2 + thickness, y2,
        x2, y2,
        
        // Box 3 (bottom)
        x1, y2,
        x2, y2,
        x2, y2 + thickness,
        x1, y2 + thickness,

        // Box 4 (left)
        x1, y1,
        x1 + thickness, y1,
        x1 + thickness, y2,
        x1, y2
    };
    /* clang-format on */

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, boxRenderable.getPositionVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(GLfloat), vertexData.data());

    // Define colors (solid white)
    const std::vector<GLfloat> colorData = {
        1.f, 1.f, 1.f, 1.f,  // box 1
        1.f, 1.f, 1.f, 1.f,  // box 2
        1.f, 1.f, 1.f, 1.f,  // box 3
        1.f, 1.f, 1.f, 1.f   // box 4
    };

    // Upload color data
    glBindBuffer(GL_ARRAY_BUFFER, boxRenderable.getColorVbo());
    glBufferSubData(GL_ARRAY_BUFFER, 0, colorData.size() * sizeof(GLfloat), colorData.data());

    // Render
    const int numIndices = boxRenderable.getIndicesPerBox() * numBoxes;
    glDrawElements(boxRenderable.getDrawMode(), numIndices, GL_UNSIGNED_INT, nullptr);
}

}  // namespace Rival
