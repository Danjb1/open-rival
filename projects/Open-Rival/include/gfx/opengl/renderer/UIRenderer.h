#pragma once

#include <glm/vec2.hpp>

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <variant>

#include "gfx/opengl/renderable/AtlasRenderable.h"
#include "gfx/opengl/renderable/BoxRenderable.h"
#include "gfx/opengl/renderable/TextRenderable.h"
#include "ui/UIVisual.h"

namespace Rival {

class Texture;
class TextureStore;
class UIElement;
class UIRoot;
class Window;

/**
 * Renders an entire UI.
 *
 * Elements are drawn in tree order (parents before children, earlier siblings before later ones), so that elements
 * further down the tree are drawn on top.
 *
 * A renderable is kept for each element with a visual, and only re-uploaded when that element's visual or bounds have
 * changed since the last frame.
 */
class UIRenderer
{
public:
    UIRenderer(const TextureStore& textureStore, const Window* window);

    void render(const UIRoot& uiRoot);

private:
    /* clang-format off */
    using Renderable = std::variant<std::unique_ptr<BoxRenderable>,
            std::unique_ptr<AtlasRenderable>,
            std::unique_ptr<TextRenderable>>;
    /* clang-format on */

    /** Everything we keep for a single element between frames. */
    struct RenderData
    {
        UIVisual visual;
        glm::ivec2 position;
        glm::ivec2 size;
        Renderable renderable;
    };

    void setUpShaders() const;
    void renderElement(const UIElement& element, const glm::ivec2& parentPosition);
    void renderVisual(int elementId, const UIVisual& visual, const glm::ivec2& position, const glm::ivec2& size);
    static Renderable createRenderable(const UIVisual& visual);
    static void renderBox(const BoxVisual& box, RenderData& entry, bool needsUpload);
    static void renderImage(const ImageVisual& image, RenderData& entry, bool needsUpload);
    static void renderText(const TextVisual& text, RenderData& entry, bool needsUpload);
    void cleanUpStaleEntries();

private:
    const Window* window;
    std::shared_ptr<const Texture> paletteTexture;

    /** RenderData by element ID. */
    std::unordered_map<int, RenderData> entries;

    /** IDs of the elements drawn this frame. */
    std::unordered_set<int> drawnElementIds;
};

}  // namespace Rival
