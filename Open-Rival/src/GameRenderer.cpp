#include "pch.h"
#include "GameRenderer.h"

#include <gl/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

#include "RenderUtils.h"
#include "Shaders.h"

namespace Rival {

    // Framebuffer size, in pixels.
    // We divide by 2 because our tiles overlap (see RenderUtils).
    const int GameRenderer::framebufferWidth =
            RenderUtils::tileWidthPx * RenderUtils::maxTilesX / 2;
    const int GameRenderer::framebufferHeight =
            RenderUtils::tileHeightPx * RenderUtils::maxTilesY / 2;

    GameRenderer::GameRenderer(
            const Window& window,
            const Scenario& scenario,
            const Camera& camera,
            const Rect& viewport,
            const Resources& res)
        : window(window),
          scenario(scenario),
          camera(camera),
          viewport(viewport),
          res(res),
          gameFbo(framebufferWidth, framebufferHeight, true),
          gameFboRenderer(gameFbo),

          tileRenderer(res.getTileSpritesheet(
                               scenario.isWilderness()),
                  res.getPalette()),

          // Hardcode the race for now
          mapBorderRenderer(Race::Human,
                  scenario.getWidth(),
                  scenario.getHeight(),
                  res.getMapBorderSpritesheet(),
                  res.getPalette()),

          entityRenderer(res.getPalette()),

          uiRenderer(res) {}

    void GameRenderer::render() const {
        // Render to our framebuffer.
        // Here the viewport specifies the region of the framebuffer texture
        // that we render onto, in pixels. We use the camera size here; if the
        // camera is wider, more pixels are visible, and thus we need a larger
        // render target.
        glBindFramebuffer(GL_FRAMEBUFFER, gameFbo.getId());
        int canvasWidth = RenderUtils::getCanvasWidth(camera.getWidth());
        int canvasHeight = RenderUtils::getCanvasHeight(camera.getHeight());
        glViewport(0, 0, canvasWidth, canvasHeight);
        renderGame(canvasWidth, canvasHeight);

        // Render the framebuffer to the screen.
        // Here the viewport specifies the region of the game window that we
        // render onto, in pixels.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(
                static_cast<int>(viewport.x),
                // Adjust for OpenGL origin
                static_cast<int>(window.getHeight()
                        - (viewport.y + viewport.height)),
                static_cast<int>(viewport.width),
                static_cast<int>(viewport.height));
        renderFramebuffer(canvasWidth, canvasHeight);

        // Render the UI to the screen
        renderUi();
    }

    void GameRenderer::renderGame(int viewportWidth, int viewportHeight) const {

        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing so we can rely on z co-ordinates for sprite
        // ordering
        glEnable(GL_DEPTH_TEST);

        // Use game world shader
        glUseProgram(Shaders::gameWorldShader.programId);

        // Determine view matrix.
        //
        // OpenGL uses right-handed rule:
        //  - x points right
        //  - y points up
        //  - z points out of the screen
        //
        // The camera co-ordinates are in camera units, but our vertices are
        // positioned using pixels. Therefore we need to convert the camera
        // co-ordinates to pixels, too.
        float cameraX = RenderUtils::cameraToPx_X(camera.getX());
        float cameraY = RenderUtils::cameraToPx_Y(camera.getY());
        float cameraZ = RenderUtils::zCamera;
        glm::mat4 view = glm::lookAt(
                glm::vec3(cameraX, cameraY, cameraZ),  // camera position
                glm::vec3(cameraX, cameraY, 0),        // look at
                glm::vec3(0, 1, 0)                     // up vector
        );

        // Determine projection matrix.
        // The projection size must match the viewport size *exactly* in order
        // to achieve pixel-perfect rendering. Any differences here could
        // result in seams between tiles.
        float left = -viewportWidth / 2.0f;
        float top = -viewportHeight / 2.0f;
        float right = viewportWidth / 2.0f;
        float bottom = viewportHeight / 2.0f;
        float near = RenderUtils::nearPlane;
        float far = RenderUtils::farPlane;
        glm::mat4 projection = glm::ortho(left, right, bottom, top, near, far);

        // Combine matrices
        glm::mat4 viewProjMatrix = projection * view;

        // Set uniform values
        glUniformMatrix4fv(Shaders::gameWorldShader.viewProjMatrixUniformLoc,
                1, GL_FALSE, &viewProjMatrix[0][0]);
        glUniform1i(Shaders::gameWorldShader.texUnitUniformLoc, 0);
        glUniform1i(Shaders::gameWorldShader.paletteTexUnitUniformLoc, 1);

        // Render Tiles
        tileRenderer.render(
                camera,
                scenario.getTiles(),
                scenario.getWidth(),
                scenario.getHeight());

        // Render Map Borders
        mapBorderRenderer.render();

        // Render Entities
        entityRenderer.render(camera, scenario.getEntities());
    }

    void GameRenderer::renderFramebuffer(int srcWidth, int srcHeight) const {

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

    void GameRenderer::renderUi() const {

        // Disable depth testing since we can trivially manage the rendering
        // order ourselves
        glDisable(GL_DEPTH_TEST);

        // Use menu shader
        glUseProgram(Shaders::menuShader.programId);

        // Set uniform values
        glUniform1i(Shaders::menuShader.texUnitUniformLoc, 0);
        glUniform1i(Shaders::menuShader.paletteTexUnitUniformLoc, 1);

        // Render the UI directly below the game world, using screen pixels
        glViewport(0, 0, window.getWidth(), window.getHeight());
        uiRenderer.renderUi(static_cast<int>(viewport.height));
    }

}  // namespace Rival
