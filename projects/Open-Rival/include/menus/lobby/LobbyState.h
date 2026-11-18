#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "application/State.h"
#include "game/PlayerState.h"
#include "net/ClientInfo.h"
#include "net/packets/Packet.h"
#include "scenario/ScenarioData.h"

namespace Rival {

class Application;
class PacketHandler;
class UIElement;
class UIRoot;

/**
 * State active while in the multiplayer lobby.
 */
class LobbyState : public State
{
public:
    LobbyState(Application& app, std::string playerName, bool host);
    ~LobbyState() = default;

    // Begin State override
    void onLoad() override;
    void onKeyEvent(const KeyEvent& evt) override;
    void update(int delta) override;
    void render(const FrameTiming& frameTiming) override;
    // End State override

    void onPlayerJoinRequest(int requestId, int clientId, const std::string& playerName);
    void onPlayerAccepted(int requestId, int clientId, const ClientInfo& client);
    void onPlayerRejected(int requestId, const std::string& playerName);
    void onWelcomeReceived(int playerId, std::unordered_map<int, ClientInfo> clients, unsigned int randomSeed);
    void onPlayerKicked(int playerId);
    void startGame();

    bool isHost() const;
    std::string getLocalPlayerName() const;
    std::unordered_map<int, ClientInfo> getClients() const;

private:
    std::unique_ptr<UIRoot> createUI();
    std::vector<std::string> getPlayerNames() const;
    /** Rebuilds the player list; should be called whenever a player joins or leaves. */
    void refreshPlayerList();
    void pollNetwork();
    int requestPlayerId();
    void loadLevel(const std::string& filename);
    void requestStartGame();
    std::unique_ptr<State> createGameState() const;

private:
    unsigned int randomSeed = 0;

    bool bIsHost;
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

    bool isGameStarted = false;

    /** Container holding a Label for each player (set by createUI, so must be declared before uiRoot). */
    UIElement* playerList = nullptr;

    std::unique_ptr<UIRoot> uiRoot;
};

}  // namespace Rival
