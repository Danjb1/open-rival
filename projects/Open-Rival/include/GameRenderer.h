#pragma once

#include "EntityRenderer.h"
#include "Framebuffer.h"
#include "FramebufferRenderer.h"
#include "MapBorderRenderer.h"
#include "TileRenderer.h"
#include "UiRenderer.h"

namespace Rival {

class Camera;
class PlayerState;
class Rect;
class TextureStore;
class Window;
class World;

class GameRenderer
{
public:
    GameRenderer(
            const Window& window,
            const World& world,
            const PlayerState& playerState,
            const Camera& camera,
            const Rect& viewport,
            const TextureStore& textureStore);

    void render(int delta);

private:
    // Framebuffer size, in pixels.
    // This is our canvas; we can never render more pixels than this.
    // We divide by 2 because our tiles overlap (see RenderUtils).
    static constexpr int framebufferWidth = RenderUtils::tileWidthPx * RenderUtils::maxTilesX / 2;
    static constexpr int framebufferHeight = RenderUtils::tileHeightPx * RenderUtils::maxTilesY / 2;

    const Window& window;
    const World& world;
    const Rect& viewport;
    const Camera& camera;
    const TextureStore& textureStore;

    /**
     * Framebuffer to which the visible region of the game is rendered at
     * a pixel-perfect scale.
     *
     * This ensures that there are no seams between tiles, and we can
     * perform any scaling when we render the framebuffer to the screen.
     */
    Framebuffer gameFbo;

    // Renderers
    EntityRenderer entityRenderer;
    FramebufferRenderer gameFboRenderer;
    TileRenderer tileRenderer;
    MapBorderRenderer mapBorderRenderer;
    UiRenderer uiRenderer;

    void renderGameViaFramebuffer(int delta) const;
    void renderGame(int viewportWidth, int viewportHeight, int delta) const;
    void renderFramebuffer(int srcWidth, int srcHeight) const;
    void renderUi();
};

}  // namespace Rival
