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
#include "gfx/renderer/GameRenderer.h"
#include "utils/Rect.h"

namespace Rival {

GameRenderer::GameRenderer(
        const Window* /*window*/,
        const World& /*world*/,
        const PlayerStore& /*playerStore*/,
        const Camera& /*camera*/,
        const Rect& /*viewport*/,
        const Resources& /*res*/,
        const PlayerContext& /*playerContext*/)
{
}

void GameRenderer::render(int /*delta*/) {}

}  // namespace Rival
