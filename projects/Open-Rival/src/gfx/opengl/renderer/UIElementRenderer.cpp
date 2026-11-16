#include "gfx/opengl/renderer/UIElementRenderer.h"

#include "gfx/opengl/GLWrapper.h"

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

void UIElementRenderer::renderBox(const BoxVisual& box, RenderData& entry, bool needsUpload)
{
    BoxRenderable& renderable = *entry.boxRenderable;

    if (needsUpload)
    {
        const glm::vec2 topLeft = entry.position;
        const glm::vec2 bottomRight = entry.position + entry.size;
        renderable.setBoxes({ { topLeft, bottomRight, box.color } });
    }

    renderable.draw();
}

void UIElementRenderer::cleanUpStaleEntries()
{
    // Any element that wasn't drawn this frame has been removed (or has lost its visual)
    std::erase_if(entries, [this](const auto& pair) { return !drawnElementIds.contains(pair.first); });
}

}  // namespace Rival
