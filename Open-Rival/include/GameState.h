#pragma once

#include <SDL.h>

#include <iostream>
#include <memory>

#include "Camera.h"
#include "Font.h"  // TMP
#include "GameRenderer.h"
#include "MenuTextRenderer.h"  // TMP
#include "MousePicker.h"
#include "Rect.h"
#include "State.h"
#include "TextRenderable.h"  // TMP
#include "World.h"

namespace Rival {

class Application;

class GameState : public State
{

public:
    GameState(Application& app, std::unique_ptr<World> world);

    // Begin State override
    void onLoad() override;
    void keyDown(const SDL_Keycode keyCode) override;
    void mouseUp(const SDL_MouseButtonEvent evt) override;
    void mouseWheelMoved(const SDL_MouseWheelEvent evt) override;
    void render(int delta) override;
    void update() override;
    // End State override

    World& getWorld()
    {
        return *world;
    }

private:
    void earlyUpdateEntities() const;
    void updateEntities() const;
    void respondToMouseInput();

private:
    /**
     * The current World.
     */
    std::unique_ptr<World> world;

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
};

}  // namespace Rival
