#ifndef MENU_TEXT_RENDERER_H
#define MENU_TEXT_RENDERER_H

#include "TextRenderable.h"
#include "TextRenderer.h"
#include "Window.h"

namespace Rival {

    class MenuTextRenderer {
    public:
        MenuTextRenderer::MenuTextRenderer(const Window& window);

        void MenuTextRenderer::render(const TextRenderable& text) const;

    private:
        const Window* window;

        TextRenderer textRenderer;
    };

}  // namespace Rival

#endif  // MENU_TEXT_RENDERER_H
