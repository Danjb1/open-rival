#include "pch.h"

#include "GameRenderer.h"

#include <gl/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

#include "Camera.h"
#include "Rect.h"
#include "RenderUtils.h"
#include "Resources.h"
#include "Shaders.h"
#include "Window.h"
#include "World.h"

namespace Rival {

GameRenderer::GameRenderer(
        const Window& window, const World& world, const Camera& camera, const Rect& viewport, const Resources& res)
    : window(window)
    , world(world)
    , camera(camera)
    , viewport(viewport)
    , res(res)
    , gameFbo(framebufferWidth, framebufferHeight, true)
    , gameFboRenderer(gameFbo)
    , tileRenderer(res.getTileSpritesheet(world.isWilderness()), res.getPalette())
    , mapBorderRenderer(
              // Hardcode the race for now
              Race::Human,
              world.getWidth(),
              world.getHeight(),
              res.getMapBorderSpritesheet(),
              res.getPalette())
    , entityRenderer(res.getPalette())
    , uiRenderer(
              // Hardcode the race for now
              Race::Human,
              res)
{
}

void GameRenderer::render(int delta)
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
    glViewport(
            static_cast<int>(viewport.x),
            // Adjust for OpenGL origin
            static_cast<int>(window.getHeight() - (viewport.y + viewport.height)),
            static_cast<int>(viewport.width),
            static_cast<int>(viewport.height));
    renderFramebuffer(canvasWidth, canvasHeight);

    // Render the UI to the screen
    renderUi();
}

void GameRenderer::renderGame(int viewportWidth, int viewportHeight, int delta) const
{

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing so we can rely on z co-ordinates for sprite
    // ordering
    glEnable(GL_DEPTH_TEST);

    // Use indexed texture shader
    glUseProgram(Shaders::indexedTextureShader.programId);

    // Project to game world
    glm::mat4 viewProjMatrix = RenderUtils::createGameProjection(camera, viewportWidth, viewportHeight);

    // Set uniform values
    glUniformMatrix4fv(Shaders::indexedTextureShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::indexedTextureShader.texUnitUniformLoc, 0);
    glUniform1i(Shaders::indexedTextureShader.paletteTexUnitUniformLoc, 1);

    // Render Tiles
    tileRenderer.render(camera, world.getTiles(), world.getWidth(), world.getHeight());

    // Render Map Borders
    mapBorderRenderer.render();

    // Render Entities
    entityRenderer.render(camera, world.getEntities(), delta);
}

void GameRenderer::renderFramebuffer(int srcWidth, int srcHeight) const
{

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Disable depth testing since we are rendering a single texture
    // directly to the screen
    glDisable(GL_DEPTH_TEST);

    // Use screen shader
    glUseProgram(Shaders::screenShader.programId);

    // Set uniform values
    glUniform1i(Shaders::screenShader.texUnitUniformLoc, 0);

    // Render framebuffer to screen.
    // At a zoom level of 1, this will result in pixel-perfect rendering.
    // A higher zoom level will result in a smaller sample, which will
    // then be stretched to fill the viewport.
    gameFboRenderer.render(srcWidth, srcHeight);
}

void GameRenderer::renderUi()
{

    // Disable depth testing since we can trivially manage the rendering
    // order ourselves
    glDisable(GL_DEPTH_TEST);

    // Use indexed texture shader
    glUseProgram(Shaders::indexedTextureShader.programId);

    // Project to menu
    glm::mat4 viewProjMatrix = RenderUtils::createMenuProjection(window.getAspectRatio());

    // Set uniform values
    glUniformMatrix4fv(Shaders::indexedTextureShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::indexedTextureShader.texUnitUniformLoc, 0);
    glUniform1i(Shaders::indexedTextureShader.paletteTexUnitUniformLoc, 1);

    // Render the UI to the screen
    glViewport(0, 0, window.getWidth(), window.getHeight());
    uiRenderer.renderUi();
}

}  // namespace Rival
