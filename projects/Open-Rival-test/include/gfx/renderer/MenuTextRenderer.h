/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub declaration for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#pragma once

#include <vector>

#include "gfx/renderable/TextRenderable.h"

class Window;

namespace Rival {

class MenuTextRenderer
{
public:
    MenuTextRenderer(const Window* window);

    void render(const std::vector<const TextRenderable*> textRenderables) const;
};

}  // namespace Rival
