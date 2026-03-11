#pragma once

#include "gfx/opengl/renderer/MenuRenderer.h"
#include "gfx/opengl/renderer/MenuTextRenderer.h"

namespace Rival {

class LobbyState;
class Resources;
class Window;

class LobbyRenderer
{
public:
    LobbyRenderer(LobbyState& lobby, const Window* window);
    void render(int delta);

private:
    void renderText();

private:
    LobbyState& lobby;
    const Resources& res;

    MenuRenderer menuRenderer;
    MenuTextRenderer textRenderer;
};

}  // namespace Rival
