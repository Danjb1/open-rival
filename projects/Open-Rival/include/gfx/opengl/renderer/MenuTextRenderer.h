#pragma once

#include <vector>

#include "gfx/opengl/renderable/TextRenderable.h"

namespace Rival {

class Window;

/**
 * Class responsible for rendering TextRenderables on the UI layer.
 *
 * TODO: Remove this once we migrate the game UI to use the new UI system.
 */
class MenuTextRenderer
{
public:
    MenuTextRenderer(const Window* window);

    void render(const std::vector<const TextRenderable*> textRenderables) const;

private:
    const Window* window;
};

}  // namespace Rival
