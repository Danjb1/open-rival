#pragma once

#include <glm/mat4x4.hpp>

#include "gfx/Framebuffer.h"
#include "gfx/renderer/DragSelectRenderer.h"
#include "gfx/renderer/EntityOverlayRenderer.h"
#include "gfx/renderer/EntityRenderer.h"
#include "gfx/renderer/FramebufferRenderer.h"
#include "gfx/renderer/MapBorderRenderer.h"
#include "gfx/renderer/TileRenderer.h"
#include "gfx/renderer/UiRenderer.h"

namespace Rival {

class Camera;
class PlayerState;
class PlayerStore;
class Rect;
class Resources;
class Window;
class World;
struct PlayerContext;

class GameRenderer
{
public:
    GameRenderer(const Window* window,
            const World& world,
            const PlayerStore& playerStore,
            const Camera& camera,
            const Rect& viewport,
            const Resources& res,
            const PlayerContext& playerContext);

    void render(int delta);

private:
    void renderGameViaFramebuffer(int delta);
    void renderGame(int viewportWidth, int viewportHeight, int delta);
    void renderGameOverlays(const glm::mat4& viewProjMatrix);
    void renderFramebuffer(int srcWidth, int srcHeight) const;
    void renderUi();
    void renderText();
    void renderDragSelect();
    void renderCursor(int delta);

private:
    // Framebuffer size, in pixels.
    // This is our canvas; we can never render more pixels than this.
    // We divide by 2 because our tiles overlap (see RenderUtils).
    static constexpr int framebufferWidth = RenderUtils::tileWidthPx * RenderUtils::maxTilesX / 2;
    static constexpr int framebufferHeight = RenderUtils::tileHeightPx * RenderUtils::maxTilesY / 2;

    const Window* window;
    const World& world;
    const Rect& viewport;
    const Camera& camera;
    const PlayerContext& playerContext;

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
    EntityOverlayRenderer entityOverlayRenderer;
    FramebufferRenderer gameFboRenderer;
    TileRenderer tileRenderer;
    MapBorderRenderer mapBorderRenderer;
    UiRenderer uiRenderer;
    DragSelectRenderer dragSelectRenderer;
};

}  // namespace Rival
