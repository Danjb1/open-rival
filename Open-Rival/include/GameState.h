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

#include "Font.h"              // TMP
#include "MenuTextRenderer.h"  // TMP
#include "TextRenderable.h"    // TMP

namespace Rival {

    class GameState : public State {

    public:
        GameState(Application& app, std::unique_ptr<Scenario> scenario);

        // Begin State override
        void onLoad() override;
        void keyDown(const SDL_Keycode keyCode) override;
        void mouseUp(const SDL_MouseButtonEvent evt) override;
        void mouseWheelMoved(const SDL_MouseWheelEvent evt) override;
        void render(int delta) override;
        void update() override;
        // End State override

        Scenario& getScenario() {
            return *scenario;
        }

    private:
        /**
         * The current Scenario.
         */
        std::unique_ptr<Scenario> scenario;

        /**
         * The rectangle on the screen to which the game is rendered (pixels).
         */
        Rect viewport;

        /**
         * The camera that defines the area of the game world to render.
         */
        Camera camera;

        /**
         * Object used to find what's under the mouse.
         */
        MousePicker mousePicker;

        /**
         * Object that renders the game.
         */
        GameRenderer gameRenderer;

        // TMP
        std::unique_ptr<TextRenderable> text1;
        std::unique_ptr<TextRenderable> text2;
        std::vector<TextRenderable*> texts;
        MenuTextRenderer textRenderer;

        void earlyUpdateEntities() const;
        void updateEntities() const;
        void respondToMouseInput();
    };

}  // namespace Rival

#endif  // RIVAL_H
