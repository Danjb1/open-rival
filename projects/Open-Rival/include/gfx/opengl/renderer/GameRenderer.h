#pragma once

#include <glm/mat4x4.hpp>

#include "gfx/opengl/Framebuffer.h"
#include "gfx/opengl/renderer/DragSelectRenderer.h"
#include "gfx/opengl/renderer/EntityOverlayRenderer.h"
#include "gfx/opengl/renderer/EntityRenderer.h"
#include "gfx/opengl/renderer/FramebufferRenderer.h"
#include "gfx/opengl/renderer/MapBorderRenderer.h"
#include "gfx/opengl/renderer/TileRenderer.h"
#include "gfx/opengl/renderer/UiRenderer.h"

namespace Rival {

class Camera;
class GameState;
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
    GameRenderer(const GameState& game,
            const Window* window,
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
    void renderMessages();

private:
    // Framebuffer size, in pixels.
    // This is our canvas; we can never render more pixels than this.
    // We divide by 2 because our tiles overlap (see RenderUtils).
    static constexpr int framebufferWidth = RenderUtils::tileWidthPx * RenderUtils::maxTilesX / 2;
    static constexpr int framebufferHeight = RenderUtils::tileHeightPx * RenderUtils::maxTilesY / 2;

    const GameState& game;
    const Window* window;
    const World& world;
    const Rect& viewport;
    const Resources& res;
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
