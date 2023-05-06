#pragma once

#include "SDLWrapper.h"

#include <memory>
#include <string>
#include <unordered_map>

#include "net/ClientInfo.h"
#include "net/packets/Packet.h"
#include "ui/MenuRenderer.h"
#include "MenuTextRenderer.h"
#include "PlayerState.h"
#include "ScenarioData.h"
#include "State.h"

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
    void update() override;
    void render(int delta) override;
    // End State override

    void onPlayerJoinRequest(int requestId, int clientId, const std::string& playerName);
    void onPlayerAccepted(int requestId, int clientId, const ClientInfo& client);
    void onPlayerRejected(int requestId, const std::string& playerName);
    void onWelcomeReceived(int playerId, std::unordered_map<int, ClientInfo> clients);
    void onPlayerKicked(int playerId);
    void startGame();

private:
    void renderText();
    void pollNetwork();
    int requestPlayerId();
    void loadLevel(const std::string& filename);
    void requestStartGame();
    std::unique_ptr<State> createGameState() const;

private:
    bool host;
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
};

}  // namespace Rival
