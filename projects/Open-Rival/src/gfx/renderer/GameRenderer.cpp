#include "gfx/renderer/GameRenderer.h"

#include "gfx/GlewWrapper.h"
#include <glm/vec3.hpp>
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

#include "application/Resources.h"
#include "application/Window.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"
#include "game/World.h"
#include "gfx/Camera.h"
#include "gfx/Palette.h"
#include "gfx/RenderUtils.h"
#include "gfx/Shaders.h"
#include "utils/Rect.h"

namespace Rival {

GameRenderer::GameRenderer(const Window* window,
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
    , playerContext(playerContext)
    , gameFbo(framebufferWidth, framebufferHeight, true)
    , gameFboRenderer(gameFbo)
    , tileRenderer(res.getTileSpritesheet(world.isWilderness()), res.getPalette())
    , mapBorderRenderer(playerStore.getLocalPlayerState().getRace(),
              world.getWidth(),
              world.getHeight(),
              res.getMapBorderSpritesheet(),
              res.getPalette())
    , entityRenderer(res, playerContext, playerStore)
    , entityOverlayRenderer(res, playerContext)
    , uiRenderer(playerStore, res, res, window, playerContext)
    , dragSelectRenderer(playerContext)
{
}

void GameRenderer::render(int delta)
{
    renderGameViaFramebuffer(delta);
    renderUi();
    renderText();
    renderDragSelect();
    renderCursor(delta);
}

void GameRenderer::renderGameViaFramebuffer(int delta)
{
    // Render to our framebuffer.
    // Here the viewport specifies the region of the framebuffer texture
    // that we render onto, in pixels. We use the camera size here; if the
    // camera is wider, more pixels are visible, and thus we need a larger
    // render target.
    glBindFramebuffer(GL_FRAMEBUFFER, gameFbo.getId());
    int canvasWidth = RenderUtils::getCanvasWidth(camera.getWidth());
    int canvasHeight = RenderUtils::getCanvasHeight(camera.getHeight());
    glViewport(0, 0, canvasWidth, canvasHeight);
    renderGame(canvasWidth, canvasHeight, delta);

    // Render the framebuffer to the screen.
    // Here the viewport specifies the region of the game window that we
    // render onto, in pixels.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(static_cast<int>(viewport.x),
            // Adjust for OpenGL origin
            static_cast<int>(window->getHeight() - (viewport.y + viewport.height)),
            static_cast<int>(viewport.width),
            static_cast<int>(viewport.height));
    renderFramebuffer(canvasWidth, canvasHeight);
}

void GameRenderer::renderGame(int viewportWidth, int viewportHeight, int delta)
{
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing so we can rely on z co-ordinates for sprite
    // ordering
    glEnable(GL_DEPTH_TEST);

    // Use indexed texture shader
    glUseProgram(Shaders::worldShader.programId);

    // Determine our view-projection matrix
    glm::mat4 viewProjMatrix = RenderUtils::createGameViewProjectionMatrix(camera, viewportWidth, viewportHeight);

    // Set uniform values
    glUniformMatrix4fv(Shaders::worldShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::worldShader.texUnitUniformLoc, 0);
    glUniform1i(Shaders::worldShader.paletteTexUnitUniformLoc, 1);
    glUniform1f(Shaders::worldShader.transparentIndex, Palette::globalTransparentColor);  // Enable transparency

    // Render Tiles
    tileRenderer.render(delta, camera, world.getTiles(), world.getWidth(), world.getHeight());

    // Render Map Borders
    mapBorderRenderer.render();

    // Render Entities
    entityRenderer.render(camera, world, delta);

    // Render overlays
    renderGameOverlays(viewProjMatrix);
}

void GameRenderer::renderGameOverlays(const glm::mat4& viewProjMatrix)
{
    // Disable depth testing since the overlays are always on top
    glDisable(GL_DEPTH_TEST);

    entityOverlayRenderer.prepare(world);

    glUseProgram(Shaders::boxShader.programId);
    glUniformMatrix4fv(Shaders::boxShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    entityOverlayRenderer.renderBoxes();

    glUseProgram(Shaders::indexedTextureShader.programId);
    glUniformMatrix4fv(Shaders::indexedTextureShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1f(Shaders::indexedTextureShader.transparentIndex, -1);  // Disable transparency
    entityOverlayRenderer.renderTextures();
}

void GameRenderer::renderFramebuffer(int srcWidth, int srcHeight) const
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Disable depth testing since we are rendering a single texture
    // directly to the screen
    glDisable(GL_DEPTH_TEST);

    // Use texture shader
    glUseProgram(Shaders::textureShader.programId);

    // Set uniform values
    glUniformMatrix4fv(Shaders::textureShader.viewProjMatrixUniformLoc,
            1,
            GL_FALSE,
            &FramebufferRenderer::viewProjectionMatrix[0][0]);
    glUniform1i(Shaders::textureShader.texUnitUniformLoc, 0);

    // Render framebuffer to screen.
    // At a zoom level of 1, this will result in pixel-perfect rendering.
    // A higher zoom level will result in a smaller sample, which will
    // then be stretched to fill the viewport.
    gameFboRenderer.render(srcWidth, srcHeight);
}

void GameRenderer::renderUi()
{
    // Disable depth testing since we can trivially manage the rendering order ourselves
    glDisable(GL_DEPTH_TEST);

    // Use indexed texture shader
    glUseProgram(Shaders::indexedTextureShader.programId);

    // Determine our view-projection matrix
    glm::mat4 viewProjMatrix = RenderUtils::createMenuViewProjectionMatrix(window->getAspectRatio());

    // Set uniform values
    glUniformMatrix4fv(Shaders::indexedTextureShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::indexedTextureShader.texUnitUniformLoc, 0);
    glUniform1i(Shaders::indexedTextureShader.paletteTexUnitUniformLoc, 1);

    // Render the UI to the screen
    glViewport(0, 0, window->getWidth(), window->getHeight());
    uiRenderer.renderUi();
}

void GameRenderer::renderText()
{
    // Disable depth testing since text is always on top
    glDisable(GL_DEPTH_TEST);

    // Render the UI to the screen
    // (the MenuTextRenderer takes care of the shader, projection, etc.)
    glViewport(0, 0, window->getWidth(), window->getHeight());
    uiRenderer.renderText();
}

void GameRenderer::renderDragSelect()
{
    if (!playerContext.dragSelect.isValid())
    {
        return;
    }

    // Disable depth testing since the drag-select is always on top
    glDisable(GL_DEPTH_TEST);

    // Use box shader
    glUseProgram(Shaders::boxShader.programId);

    // Determine our view-projection matrix
    glm::mat4 viewProjMatrix = RenderUtils::createMenuViewProjectionMatrix(window->getAspectRatio());

    // Set uniform values
    glUniformMatrix4fv(Shaders::boxShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);

    // Render the drag-select to the screen
    glViewport(0, 0, window->getWidth(), window->getHeight());
    dragSelectRenderer.render();
}

void GameRenderer::renderCursor(int delta)
{
    // Disable depth testing since the cursor is always on top
    glDisable(GL_DEPTH_TEST);

    // Use indexed texture shader
    glUseProgram(Shaders::indexedTextureShader.programId);

    // Determine our view-projection matrix
    glm::mat4 viewProjMatrix = RenderUtils::createMenuViewProjectionMatrix(window->getAspectRatio());

    // Set uniform values
    glUniformMatrix4fv(Shaders::indexedTextureShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::indexedTextureShader.texUnitUniformLoc, 0);
    glUniform1i(Shaders::indexedTextureShader.paletteTexUnitUniformLoc, 1);
    glUniform1f(Shaders::indexedTextureShader.transparentIndex, Palette::cursorTransparentColor);

    // Render the cursor to the screen
    glViewport(0, 0, window->getWidth(), window->getHeight());
    uiRenderer.renderCursor(delta);
}

}  // namespace Rival
