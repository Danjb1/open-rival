#pragma once

#include "utils/SDLWrapper.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "application/State.h"
#include "commands/GameCommand.h"
#include "game/MousePicker.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"
#include "game/World.h"
#include "gfx/Camera.h"
#include "gfx/renderer/GameRenderer.h"
#include "net/ClientInfo.h"
#include "net/packet-handlers/PacketHandler.h"
#include "net/packets/Packet.h"
#include "utils/Rect.h"

namespace Rival {

class Application;
class PacketHandler;

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
    GameState(Application& app,
            std::unique_ptr<World> world,
            std::unordered_map<int, PlayerState>& playerStates,
            std::unordered_map<int, ClientInfo> clients,
            int localPlayerId);

    // Begin State override
    void onLoad() override;
    void keyDown(const SDL_Keycode keyCode) override;
    void keyUp(const SDL_Keycode keyCode) override;
    void mouseDown(const SDL_MouseButtonEvent event) override;
    void mouseUp(const SDL_MouseButtonEvent event) override;
    void mouseWheelMoved(const SDL_MouseWheelEvent event) override;
    void update() override;
    void render(int delta) override;
    // End State override

    // Begin WorldStore override
    World& getWorld() override;
    // End WorldStore override

    // Begin PlayerStore override
    int getNumPlayers() const override;
    PlayerState& getLocalPlayerState() const override;
    PlayerState* getPlayerState(int playerId) const override;
    bool isLocalPlayer(int playerId) const override;
    bool isSameTeam(int playerId) const override;
    // End PlayerStore override

    // Begin GameCommandInvoker override
    void dispatchCommand(std::shared_ptr<GameCommand> command) override;
    // End GameCommandInvoker override

    void scheduleCommand(std::shared_ptr<GameCommand> command, int tick);
    void onClientReady(int tick, int clientId);

private:
    bool isTickReady() const;
    void pollNetwork();
    void earlyUpdateEntities() const;
    void updateEntities() const;
    void respondToInput();
    void sendOutgoingCommands();
    void processCommands();
    bool isNetGame() const;

private:
    /** Player ID of the local player.
     * In a single-player game or when hosting, this is always zero. When joining a net game, this is allocated by the
     * server. */
    int localPlayerId = 0;

    /** Map of client ID -> ClientInfo. */
    std::unordered_map<int, ClientInfo> clients;

    /** Map of player ID -> player state. */
    std::unordered_map<int, PlayerState>& playerStates;

    /** The current World. */
    std::unique_ptr<World> world;

    /** The rectangle on the screen to which the game is rendered (pixels). */
    Rect viewport;

    /** The camera that defines the area of the game world to render. */
    Camera camera;

    /** Local player context. */
    PlayerContext playerContext;

    /** Object used to find what's under the mouse. */
    MousePicker mousePicker;

    /** Object that renders the game. */
    GameRenderer gameRenderer;

    /** Registered PacketHandlers by packet type. */
    std::unordered_map<PacketType, std::unique_ptr<PacketHandler>> packetHandlers;

    /** Commands ready to be executed, indexed by the tick number when they are due. */
    std::unordered_map<int, std::vector<std::shared_ptr<GameCommand>>> pendingCommands;

    /** Clients for whom we have received commands, indexed by tick number. */
    std::unordered_map<int, std::unordered_set<int>> clientsReady;

    /** Commands queued for sending over the network. */
    std::vector<std::shared_ptr<GameCommand>> outgoingCommands;

    /** The current player input. */
    Input input = {};

    /** Tick number, incremented with each update. */
    int currentTick = 0;

    /** Flag set to true when waiting for network player input. */
    bool isWaitingForPlayers = false;
};

}  // namespace Rival
