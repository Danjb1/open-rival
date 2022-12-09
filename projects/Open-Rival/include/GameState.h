#pragma once

#include "SDLWrapper.h"

#include <iostream>
#include <memory>
#include <unordered_map>

#include "Camera.h"
#include "GameCommand.h"
#include "GameRenderer.h"
#include "MousePicker.h"
#include "PlayerContext.h"
#include "PlayerState.h"
#include "Rect.h"
#include "State.h"
#include "World.h"

namespace Rival {

class Application;

/**
 * Represents an optional direction along a given axis.
 */
enum class Direction : std::int8_t
{
    Decreasing = -1,
    None = 0,
    Increasing = 1
};

/**
 * Holds the current player input.
 */
struct Input
{
    Direction lastDirectionX = Direction::None;
    Direction lastDirectionY = Direction::None;
};

/**
 * Application state active when in-game.
 */
class GameState
    : public State
    , public PlayerStore
    , public GameCommandInvoker
    , public GameCommandContext
{

public:
    GameState(Application& app, std::unique_ptr<World> world, std::unordered_map<int, PlayerState>& playerStates);

    // Begin State override
    void onLoad() override;
    void keyDown(const SDL_Keycode keyCode) override;
    void keyUp(const SDL_Keycode keyCode) override;
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

    // Begin GameCommandInvoker override
    void dispatchCommand(std::shared_ptr<GameCommand> command) override;
    // End GameCommandInvoker override

private:
    void earlyUpdateEntities() const;
    void updateEntities() const;
    void respondToInput();
    void processCommands();

private:
    /**
     * Map of player ID -> player state.
     */
    std::unordered_map<int, PlayerState>& playerStates;

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
     * Local player context.
     */
    PlayerContext playerContext;

    /**
     * Object used to find what's under the mouse.
     */
    MousePicker mousePicker;

    /**
     * Object that renders the game.
     */
    GameRenderer gameRenderer;

    /**
     * Commands ready to be executed.
     */
    std::vector<std::shared_ptr<GameCommand>> pendingCommands;

    /**
     * The current player input.
     */
    Input input = {};

    /**
     * Player ID of the local player.
     *
     * Later, we will be allocated a player ID by the server.
     */
    int localPlayerId = 0;
};

}  // namespace Rival
