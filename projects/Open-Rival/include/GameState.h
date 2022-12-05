#pragma once

#include "SDLWrapper.h"

#include <iostream>
#include <memory>
#include <unordered_map>

#include "Camera.h"
#include "Font.h"  // TMP
#include "GameRenderer.h"
#include "MenuTextRenderer.h"  // TMP
#include "MousePicker.h"
#include "PlayerState.h"
#include "Rect.h"
#include "State.h"
#include "TextRenderable.h"  // TMP
#include "World.h"

namespace Rival {

class Application;

/**
 * Application state active when in-game.
 */
class GameState
    : public State
    , public PlayerStore
    , public WorldStore
{

public:
    GameState(Application& app, std::unique_ptr<World> world, std::unordered_map<int, PlayerState>& playerStates);

    // Begin State override
    void onLoad() override;
    void keyDown(const SDL_Keycode keyCode) override;
    void mouseDown(const SDL_MouseButtonEvent event) override;
    void mouseUp(const SDL_MouseButtonEvent event) override;
    void mouseWheelMoved(const SDL_MouseWheelEvent event) override;
    void render(int delta) override;
    void update() override;
    // End State override

    // Begin WorldStore override
    World& getWorld() override;
    // End WorldStore override

    // Begin PlayerStore override
    PlayerState& getLocalPlayerState() const override;
    PlayerState* getPlayerState(int playerId) const override;
    bool isLocalPlayer(int playerId) const override;
    // End PlayerStore override

private:
    void earlyUpdateEntities() const;
    void updateEntities() const;
    void respondToMouseInput();

private:
    /**
     * Map of player ID -> player state.
     */
    std::unordered_map<int, PlayerState>& playerStates;

    /**
     * Player ID of the local player.
     *
     * Later, we will be allocated a player ID by the server.
     */
    int localPlayerId = 0;

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
