/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "application/Resources.h"
#include "application/Window.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"
#include "game/World.h"
#include "gfx/Camera.h"
#include "gfx/Palette.h"
#include "gfx/RenderUtils.h"
#include "gfx/Shaders.h"
#include "gfx/renderer/GameRenderer.h"
#include "utils/Rect.h"

namespace Rival {

GameRenderer::GameRenderer(
        const Window* window,
        const World& world,
        const PlayerStore& playerStore,
        const Camera& camera,
        const Rect& viewport,
        const Resources& res,
        const PlayerContext& playerContext)
    : window(window)
    , world(world)
    , camera(camera)
    , viewport(viewport)
    , gameFbo(framebufferWidth, framebufferHeight, true)
    , gameFboRenderer(gameFbo)
    , tileRenderer(res.getTileSpritesheet(world.isWilderness()), res.getPalette())
    , mapBorderRenderer(
              playerStore.getLocalPlayerState().getRace(),
              world.getWidth(),
              world.getHeight(),
              res.getMapBorderSpritesheet(),
              res.getPalette())
    , entityRenderer(res, playerContext)
    , uiRenderer(playerStore, res, res, window, playerContext)
{
}

void GameRenderer::render(int /*delta*/)
{
    // Do nothing!
}

}  // namespace Rival
