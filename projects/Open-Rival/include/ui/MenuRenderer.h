#pragma once

#include "gfx/renderable/TextureRenderable.h"
#include "ui/CursorRenderer.h"
#include "game/GameInterface.h"
#include "application/Resources.h"

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
    MenuRenderer(const TextureStore& textureStore, const Window* window, const Rect& viewport);

    void render(int delta);

private:
    void renderBackground();
    void sendBackgroundDataToGpu();
    void renderCursor(int delta);

private:
    const TextureStore& textureStore;
    const Window* window;
    const Rect& viewport;

    GameInterface::UiImage backgroundImage;
    TextureRenderable backgroundRenderable;

    CursorRenderer cursorRenderer;
};

}  // namespace Rival
