#include "pch.h"
#include "GameState.h"

#include <map>
#include <stdexcept>

#include "GameInterface.h"
#include "Image.h"
#include "MouseUtils.h"
#include "Palette.h"
#include "Race.h"
#include "RenderUtils.h"
#include "Spritesheet.h"

#include "Shaders.h"  // TMP

namespace Rival {

    GameState::GameState(Application& app, std::unique_ptr<Scenario> scenarioToMove)
        : State(app),
          scenario(std::move(scenarioToMove)),
          viewport(0, 0,
                  window.getWidth(),
                  window.getHeight() - GameInterface::uiHeight),
          camera(0.0f, 0.0f,
                  RenderUtils::pxToCamera_X(
                          static_cast<float>(viewport.width)),
                  RenderUtils::pxToCamera_Y(
                          static_cast<float>(viewport.height)),
                  *scenario),
          mousePicker(camera, viewport, *scenario),
          gameRenderer(window, *scenario, camera, viewport, res),
          textRenderer(window) {

        // TMP
        font = std::make_unique<Font>(Font::loadFont("Procopius Regular.ttf"));
        std::vector<TextSpan> spans = {
            { "Hello ", TextRenderable::defaultColor },
            { "world", TextRenderable::highlightColor }
        };
        TextProperties props = { *font };
        text = std::make_unique<TextRenderable>(spans, props, 300.0f, 525.0f);
    }

    void GameState::onLoad() {
        app.getAudioSystem().playMidi(res.getMidi(0));
    }

    void GameState::update() {
        earlyUpdateEntities();
        respondToMouseInput();
        updateEntities();
        scenario->cleanUpEntities();
    }

    void GameState::earlyUpdateEntities() const {
        auto const& entities = scenario->getEntities();
        for (auto const& e : entities) {
            e->earlyUpdate();
        }
    }

    void GameState::respondToMouseInput() {
        mousePicker.handleMouse();
    }

    void GameState::updateEntities() const {
        auto const& entities = scenario->getEntities();
        for (auto const& e : entities) {
            e->update();
        }
    }

    void GameState::render() {
        gameRenderer.render();

        // TMP
        textRenderer.render(*text);
    }

    void GameState::keyDown(const SDL_Keycode keyCode) {
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

#include <cstdlib>  // TMP (for rand)
    void GameState::mouseUp(const SDL_MouseButtonEvent) {
        // TMP: play a random sound!
        int soundId = std::rand() % 369;
        const WaveFile& waveFile = res.getSound(soundId);
        app.getAudioSystem().playSound(SoundSource(waveFile));
    }

    void GameState::mouseWheelMoved(const SDL_MouseWheelEvent evt) {

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
        float zoomBefore = camera.getZoom();
        if (scrollAmount > 0) {
            camera.modZoom(Camera::zoomInterval);
        } else {
            camera.modZoom(-Camera::zoomInterval);
        }
        float zoomAfter = camera.getZoom();

        // If the zoom level hasn't changed, there is no need to translate
        // the camera
        if (zoomBefore == zoomAfter) {
            return;
        }

        // Calculate mouse position relative to the viewport, in the range 0-1
        float normalizedMouseX =
                MouseUtils::getNormalizedMouseInViewportX(mouseX, viewport);
        float normalizedMouseY =
                MouseUtils::getNormalizedMouseInViewportY(mouseY, viewport);

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
