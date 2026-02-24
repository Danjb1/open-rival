#pragma once

#include "utils/SDLWrapper.h"

#include <memory>
#include <string>
#include <unordered_map>

#include "application/State.h"
#include "game/PlayerState.h"
#include "gfx/renderer/MenuTextRenderer.h"
#include "net/ClientInfo.h"
#include "net/packets/Packet.h"
#include "scenario/ScenarioData.h"
#include "ui/MenuRenderer.h"

namespace Rival {

class Application;
class PacketHandler;

/**
 * Application state active when in-game.
 */
class LobbyState : public State
{

public:
    LobbyState(Application& app, std::string playerName, bool host);

    // Begin State override
    void onLoad() override;
    void keyUp(const SDL_Keycode keyCode) override;
    void update(int delta) override;
    void render(int delta) override;
    // End State override

    void onPlayerJoinRequest(int requestId, int clientId, const std::string& playerName);
    void onPlayerAccepted(int requestId, int clientId, const ClientInfo& client);
    void onPlayerRejected(int requestId, const std::string& playerName);
    void onWelcomeReceived(int playerId, std::unordered_map<int, ClientInfo> clients, unsigned int randomSeed);
    void onPlayerKicked(int playerId);
    void startGame();

private:
    void renderText();
    void pollNetwork();
    int requestPlayerId();
    void loadLevel(const std::string& filename);
    void requestStartGame();
    std::unique_ptr<State> createGameState() const;
    bool isNetGame() const;

private:
    unsigned int randomSeed = 0;

    bool isHost;
    int nextPlayerId = 1;
    int joinRequestId = 0;

    int localPlayerId = -1;
    std::string localPlayerName;

    /** The selected scenario. */
    ScenarioData scenarioData;

    /** Map of client ID -> ClientInfo.
     * Does not include an entry for the local player. */
    std::unordered_map<int, ClientInfo> clients;

    /** Registered PacketHandlers by packet type. */
    std::unordered_map<PacketType, std::unique_ptr<PacketHandler>> packetHandlers;

    MenuRenderer menuRenderer;
    MenuTextRenderer textRenderer;

    bool isGameStarted = false;
};

}  // namespace Rival
