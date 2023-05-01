#pragma once

#include "ui/CursorRenderer.h"

namespace Rival {

class Rect;
class Resources;
class Window;

/**
 * Class that can render a menu.
 */
class MenuRenderer
{
public:
    MenuRenderer(const Window* window, const Rect& viewport, const Resources& res);

    void render(int delta);

private:
    void renderCursor(int delta);

private:
    const Window* window;
    const Rect& viewport;

    CursorRenderer cursorRenderer;
};

}  // namespace Rival
