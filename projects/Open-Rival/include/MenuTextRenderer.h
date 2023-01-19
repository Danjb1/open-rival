#pragma once

#include <vector>

#include "TextRenderable.h"
#include "TextRenderer.h"
#include "Window.h"

namespace Rival {

/**
 * Class responsible for rendering TextRenderables on the UI layer.
 */
class MenuTextRenderer
{
public:
    MenuTextRenderer::MenuTextRenderer(const Window* window);

    void MenuTextRenderer::render(const std::vector<TextRenderable*> textRenderables) const;

private:
    const Window* window;

    TextRenderer textRenderer;
};

}  // namespace Rival
