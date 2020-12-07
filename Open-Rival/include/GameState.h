#ifndef RIVAL_H
#define RIVAL_H

#include <SDL.h>
#include <iostream>
#include <memory>

#include "Application.h"
#include "Camera.h"
#include "GameRenderer.h"
#include "MousePicker.h"
#include "Rect.h"
#include "Scenario.h"
#include "State.h"

namespace Rival {

    class GameState : public State {

    public:
        GameState(Application& app, std::unique_ptr<Scenario> scenario);

        void onLoad() override;

        void keyDown(const SDL_Keycode keyCode) override;

        void mouseWheelMoved(const SDL_MouseWheelEvent evt) override;

        void render() override;

        void update() override;

        Scenario& getScenario();

    private:
        /**
         * The current Scenario.
         */
        std::unique_ptr<Scenario> scenario;

        /**
         * The camera that defines the area of the game world to render.
         */
        Camera camera;

        /**
         * The rectangle on the screen to which the game is rendered (pixels).
         */
        Rect viewport;

        /**
         * Object used to find what's under the mouse.
         */
        MousePicker mousePicker;

        /**
         * Object that renders the game.
         */
        GameRenderer gameRenderer;

        /**
         * Gets the total height of the rendered UI.
         */
        int GameState::getUiHeight() const;
    };

}  // namespace Rival

#endif  // RIVAL_H
