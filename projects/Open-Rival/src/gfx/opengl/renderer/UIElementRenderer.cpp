#include "gfx/opengl/renderer/UIElementRenderer.h"

#include "gfx/opengl/GLWrapper.h"

#include <vector>

#include "gfx/RenderUtils.h"
#include "gfx/opengl/Shaders.h"
#include "ui/UIRoot.h"
#include "window/Window.h"

namespace Rival {

UIElementRenderer::UIElementRenderer(const Window* window)
    : window(window)
{
}

void UIElementRenderer::render(const UIRoot& uiRoot)
{
    // Disable depth testing since we are drawing in tree order
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, window->getWidth(), window->getHeight());

    // For now, all visuals are boxes, so we only need to set the shader once
    glm::mat4 viewProjMatrix = RenderUtils::createMenuViewProjectionMatrix(window->getAspectRatio());
    glUseProgram(Shaders::boxShader.programId);
    glUniformMatrix4fv(Shaders::boxShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);

    drawnElementIds.clear();
    renderElement(uiRoot, { 0, 0 });
    cleanUpStaleEntries();
}

void UIElementRenderer::renderElement(const UIElement& element, const glm::ivec2& parentPosition)
{
    const glm::ivec2 position = parentPosition + element.getPosition();

    if (const UIVisual* visual = element.getVisual())
    {
        renderVisual(element.getId(), *visual, position, element.getSize());
    }

    for (const auto& child : element.getAllChildren())
    {
        renderElement(*child, position);
    }
}

void UIElementRenderer::renderVisual(
        int elementId, const UIVisual& visual, const glm::ivec2& position, const glm::ivec2& size)
{
    drawnElementIds.insert(elementId);

    auto it = entries.find(elementId);
    bool needsUpload = false;

    // Create a new renderable if this element has never been drawn, or its visual has changed type
    if (it == entries.end() || it->second.visual.index() != visual.index())
    {
        RenderData newEntry { visual, position, size, nullptr };
        if (std::holds_alternative<BoxVisual>(visual))
        {
            newEntry.boxRenderable = std::make_unique<BoxRenderable>(1);
        }
        it = entries.insert_or_assign(elementId, std::move(newEntry)).first;
        needsUpload = true;
    }

    RenderData& entry = it->second;
    if (entry.visual != visual || entry.position != position || entry.size != size)
    {
        entry.visual = visual;
        entry.position = position;
        entry.size = size;
        needsUpload = true;
    }

    if (const BoxVisual* box = std::get_if<BoxVisual>(&visual))
    {
        renderBox(*box, entry, needsUpload);
    }
}

void UIElementRenderer::renderBox(const BoxVisual& box, const RenderData& entry, bool needsUpload)
{
    const BoxRenderable& renderable = *entry.boxRenderable;
    glBindVertexArray(renderable.getVao());

    if (needsUpload)
    {
        const float x1 = static_cast<float>(entry.position.x);
        const float y1 = static_cast<float>(entry.position.y);
        const float x2 = static_cast<float>(entry.position.x + entry.size.x);
        const float y2 = static_cast<float>(entry.position.y + entry.size.y);

        /* clang-format off */
        const std::vector<GLfloat> vertexData = {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        };
        /* clang-format on */
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getPositionVbo());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(GLfloat), vertexData.data());

        std::vector<GLfloat> colorData;
        for (int i = 0; i < BoxRenderable::numVerticesPerBox; ++i)
        {
            colorData.insert(colorData.end(), { box.color.r, box.color.g, box.color.b, box.color.a });
        }
        glBindBuffer(GL_ARRAY_BUFFER, renderable.getColorVbo());
        glBufferSubData(GL_ARRAY_BUFFER, 0, colorData.size() * sizeof(GLfloat), colorData.data());
    }

    glDrawElements(renderable.getDrawMode(), renderable.getIndicesPerBox(), GL_UNSIGNED_INT, nullptr);
}

void UIElementRenderer::cleanUpStaleEntries()
{
    // Any element that wasn't drawn this frame has been removed (or has lost its visual)
    std::erase_if(entries, [this](const auto& pair) { return !drawnElementIds.contains(pair.first); });
}

}  // namespace Rival
