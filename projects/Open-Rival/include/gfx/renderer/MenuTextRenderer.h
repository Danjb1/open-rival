#pragma once

#include <vector>

#include "gfx/renderable/TextRenderable.h"
#include "gfx/renderer/TextRenderer.h"

namespace Rival {

class Window;

/**
 * Class responsible for rendering TextRenderables on the UI layer.
 */
class MenuTextRenderer
{
public:
    MenuTextRenderer(const Window* window);

    void render(const std::vector<const TextRenderable*> textRenderables) const;

private:
    const Window* window;

    TextRenderer textRenderer;
};

}  // namespace Rival
