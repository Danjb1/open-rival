#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "gfx/opengl/renderable/BoxRenderable.h"
#include "ui/UIVisual.h"

namespace Rival {

class UIElement;
class UIRoot;
class Window;

// Later, we should rename this to UIRenderer (and deprecate the old one).
// MenuRenderer should also be deprecated.

/**
 * Renders an entire UI.
 *
 * Elements are drawn in tree order (parents before children, earlier siblings before later ones), so that elements
 * further down the tree are drawn on top.
 *
 * A renderable is kept for each element with a visual, and only re-uploaded when that element's visual or bounds have
 * changed since the last frame.
 */
class UIElementRenderer
{
public:
    explicit UIElementRenderer(const Window* window);

    void render(const UIRoot& uiRoot);

private:
    /** Everything we keep for a single element between frames. */
    struct RenderData
    {
        UIVisual visual;
        glm::ivec2 position;
        glm::ivec2 size;
        std::unique_ptr<BoxRenderable> boxRenderable;
    };

    void renderElement(const UIElement& element, const glm::ivec2& parentPosition);
    void renderVisual(int elementId, const UIVisual& visual, const glm::ivec2& position, const glm::ivec2& size);
    static void renderBox(const BoxVisual& box, const RenderData& entry, bool needsUpload);
    void cleanUpStaleEntries();

private:
    const Window* window;

    /** RenderData by element ID. */
    std::unordered_map<int, RenderData> entries;

    /** IDs of the elements drawn this frame. */
    std::unordered_set<int> drawnElementIds;
};

}  // namespace Rival
