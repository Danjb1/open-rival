#pragma once

#include "application/Resources.h"
#include "game/GameInterface.h"
#include "gfx/opengl/renderable/TextureRenderable.h"

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

    void render();

private:
    void renderBackground();
    void sendBackgroundDataToGpu();

private:
    const TextureStore& textureStore;
    const Window* window;
    const Rect& viewport;

    GameInterface::UiImage backgroundImage;
    TextureRenderable backgroundRenderable;
};

}  // namespace Rival
