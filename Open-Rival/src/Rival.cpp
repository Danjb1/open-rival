#include "pch.h"
#include "Rival.h"

// Delete <iostream> if std::cout is no longer utilised for quick-debugging
#include <iostream>

#include <gl/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <stdexcept>

#include "Image.h"
#include "Palette.h"
#include "Race.h"
#include "RenderUtils.h"
#include "Shaders.h"
#include "Spritesheet.h"
#include "TimerUtils.h"

namespace Rival {

    // Framebuffer size, in pixels.
    // We divide by 2 because our tiles overlap (see RenderUtils).
    const int Rival::framebufferWidth =
            RenderUtils::tileWidthPx * RenderUtils::maxTilesX / 2;
    const int Rival::framebufferHeight =
            RenderUtils::tileHeightPx * RenderUtils::maxTilesY / 2;

    Rival::Rival(Window& window, Scenario& scenario)
        : window(window),
          viewport(Rect(0, 0,
                  static_cast<float>(window.getWidth()),
                  static_cast<float>(window.getHeight() - 100))),
          scenario(scenario),
          camera(0.0f, 0.0f,
                  RenderUtils::pxToWorld_X(
                          static_cast<float>(window.getWidth())),
                  window.getAspectRatio(),
                  scenario) {}

    void Rival::initialize(Application* app) {

        // Create our framebuffer
        gameFbo = std::make_unique<Framebuffer>(
                Framebuffer::generate(
                        framebufferWidth, framebufferHeight, true));

        // Create the FramebufferRenderer
        gameFboRenderer = std::make_unique<FramebufferRenderer>(*gameFbo);
        gameFboRenderer->init();

        // Create the MousePicker
        mousePicker = std::make_unique<MousePicker>(camera, viewport, scenario);

        // Pick which tile Spritesheet to use based on the map type
        Resources& res = app->getResources();
        const Spritesheet& tileSpritesheet = scenario.isWilderness()
                ? res.getTileSpritesheet(1)
                : res.getTileSpritesheet(0);

        // Create the TileRenderer
        tileRenderer = std::make_unique<TileRenderer>(
                tileSpritesheet,
                res.getPalette());

        // Hardcode this, for now
        Race race = Race::Human;

        // Create the MapBorderRenderer
        mapBorderRenderer = std::make_unique<MapBorderRenderer>(
                race,
                scenario.getWidth(),
                scenario.getHeight(),
                res.getMapBorderSpritesheet(),
                res.getPalette());

        // Create the UnitRenderer
        unitRenderer = std::make_unique<UnitRenderer>(
                res.getUnitSpritesheets(),
                res.getPalette());

        // Create the BuildingRenderer
        buildingRenderer = std::make_unique<BuildingRenderer>(
                res.getBuildingSpritesheets(),
                res.getPalette());
    }

    void Rival::update() {
        mousePicker->handleMouse();

        // TODO: Is this needed? (sprites don't render properly otherwise?)
        std::map<int, std::unique_ptr<Building>>& buildings = scenario.getBuildings();
        for (auto const& kv : buildings) {
            const std::unique_ptr<Building>& building = kv.second;
            building->tick();
        }

        std::map<int, std::unique_ptr<Unit>>& units = scenario.getUnits();
        for (auto const& kv : units) {
            const std::unique_ptr<Unit>& unit = kv.second;
            unit->tick();
        }
    }

    void Rival::render() {

        // Render to our framebuffer.
        // Here the viewport specifies the region of the framebuffer texture
        // that we render onto, in pixels. We use the camera size here; if the
        // camera is wider, more pixels are visible, and thus we need a larger
        // render target.
        glBindFramebuffer(GL_FRAMEBUFFER, gameFbo->getId());
        int canvasWidth = static_cast<int>(
                RenderUtils::worldToPx_X(camera.getWidth()));
        int canvasHeight = static_cast<int>(
                RenderUtils::worldToPx_Y(camera.getHeight()));
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
    }

    void Rival::renderGame(int viewportWidth, int viewportHeight) {

        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing so we can rely on z co-ordinates for sprite
        // ordering
        glEnable(GL_DEPTH_TEST);

        // Use indexed texture shader
        glUseProgram(indexedTextureShader.programId);

        // Determine view matrix.
        // OpenGL uses right-handed rule:
        //  - x points right
        //  - y points up
        //  - z points out of the screen
        // The camera co-ordinates are in world units (tiles), but our vertices
        // are positioned using pixels. Therefore we need to convert the camera
        // co-ordinates to pixels, too.
        float cameraX = RenderUtils::worldToPx_X(camera.getX());
        float cameraY = RenderUtils::worldToPx_Y(camera.getY());
        glm::mat4 view = glm::lookAt(
                glm::vec3(cameraX, cameraY, 0),   // camera position
                glm::vec3(cameraX, cameraY, -1),  // look at
                glm::vec3(0, 1, 0)                // up vector
        );

        // Determine projection matrix.
        // This should match the viewport size exactly, in order to achieve
        // pixel-perfect rendering.
        float left = -viewportWidth / 2.0f;
        float top = -viewportHeight / 2.0f;
        float right = viewportWidth / 2.0f;
        float bottom = viewportHeight / 2.0f;
        float near = 1.0f;
        float far = 1024.0f;
        glm::mat4 projection = glm::ortho(left, right, bottom, top, near, far);

        // Combine matrices
        glm::mat4 viewProjMatrix = projection * view;

        // Set uniform values
        glUniformMatrix4fv(indexedTextureShader.viewProjMatrixUniformLocation,
                1, GL_FALSE, &viewProjMatrix[0][0]);
        glUniform1i(indexedTextureShader.texUnitUniformLocation, 0);
        glUniform1i(indexedTextureShader.paletteTexUnitUniformLocation, 1);

        // Render Tiles
        tileRenderer->render(
                camera,
                scenario.getTiles(),
                scenario.getWidth(),
                scenario.getHeight());

        // Render Map Borders
        mapBorderRenderer->render();

        // Render Units
        unitRenderer->render(camera, scenario.getUnits());

        // Render Units
        buildingRenderer->render(camera, scenario.getBuildings());
    }

    void Rival::renderFramebuffer(int srcWidth, int srcHeight) {

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Disable depth testing since we are rendering a single texture
        // directly to the screen
        glDisable(GL_DEPTH_TEST);

        // Use screen shader
        glUseProgram(screenShader.programId);

        // Set uniform values
        glUniform1i(screenShader.texUnitUniformLocation, 0);

        // Render framebuffer to screen.
        // At a zoom level of 1, this will result in pixel-perfect rendering.
        // A higher zoom level will result in a smaller sample, which will
        // then be stretched to fill the viewport.
        gameFboRenderer->render(srcWidth, srcHeight);
    }

    void Rival::keyDown(const SDL_Keycode keyCode) {
        switch (keyCode) {
        case SDLK_UP:
            camera.translate(0.0f, -0.5f);
            break;

        case SDLK_DOWN:
            camera.translate(0.0f, 0.5f);
            break;

        case SDLK_LEFT:
            camera.translate(-0.5f, 0.0f);
            break;

        case SDLK_RIGHT:
            camera.translate(0.5f, 0.0f);
            break;

        default:
            break;
        }
    }

    void Rival::mouseWheelMoved(const SDL_MouseWheelEvent evt) {

        // Get the mouse position relative to the window, in pixels
        int mouseX;
        int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Abort if the mouse is outside the viewport
        if (!viewport.contains(mouseX, mouseY)) {
            return;
        }

        // Check if any scrolling took place
        int scrollAmount = evt.y;
        if (scrollAmount == 0) {
            return;
        }

        // Determine the normalized scroll amount
        if (evt.direction == SDL_MOUSEWHEEL_FLIPPED) {
            scrollAmount *= -1;
        }

        // Zoom
        if (scrollAmount > 0) {
            camera.modZoom(Camera::zoomInterval);
        } else {
            camera.modZoom(-Camera::zoomInterval);
        }

        // Calculate the mouse position relative to the viewport, in pixels
        int mouseInViewportX = mouseX - static_cast<int>(viewport.x);
        int mouseInViewportY = mouseY - static_cast<int>(viewport.y);

        // Calculate mouse position relative to the viewport, in the range 0-1
        float normalizedMouseX =
                static_cast<float>(mouseInViewportX) / viewport.width;
        float normalizedMouseY =
                static_cast<float>(mouseInViewportY) / viewport.height;

        // Calculate mouse position relative to the viewport centre, in the
        // range -1 to 1
        float relativeMouseX = (2 * normalizedMouseX) - 1;
        float relativeMouseY = (2 * normalizedMouseY) - 1;

        // Move the camera based on the cursor position.
        // We move towards the cursor when zooming in, and away from the cursor
        // when zooming out.
        if (scrollAmount > 0) {
            camera.translate(relativeMouseX, relativeMouseY);
        } else {
            camera.translate(-relativeMouseX, -relativeMouseY);
        }
    }

}  // namespace Rival
