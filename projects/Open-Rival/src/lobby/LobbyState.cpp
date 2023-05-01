#include "pch.h"

#include "lobby/LobbyState.h"

#include <cstdlib>  // std::rand

#include "net/packet-handlers/AcceptPlayerPacketHandler.h"
#include "net/packet-handlers/KickPlayerPacketHandler.h"
#include "net/packet-handlers/LobbyWelcomePacketHandler.h"
#include "net/packet-handlers/RejectPlayerPacketHandler.h"
#include "net/packet-handlers/RequestJoinPacketHandler.h"
#include "net/packet-handlers/StartGamePacketHandler.h"
#include "net/packets/AcceptPlayerPacket.h"
#include "net/packets/KickPlayerPacket.h"
#include "net/packets/LobbyWelcomePacket.h"
#include "net/packets/RejectPlayerPacket.h"
#include "net/packets/RequestJoinPacket.h"
#include "net/packets/StartGamePacket.h"
#include "Application.h"
#include "ApplicationContext.h"
#include "ConfigUtils.h"
#include "GameState.h"
#include "PlayerState.h"
#include "ScenarioBuilder.h"
#include "ScenarioReader.h"
#include "World.h"

namespace Rival {

Rect makeViewport(const Window* window)
{
    // Fill the screen
    return { 0, 0, window->getWidth(), window->getHeight() };
}

LobbyState::LobbyState(Application& app, std::string playerName, bool host)
    : State(app)
    , host(host)
    , localPlayerName(playerName)
    , menuRenderer(window, makeViewport(window), res)
{
    // Register PacketHandlers
    packetHandlers.insert({ PacketType::RequestJoin, std::make_unique<RequestJoinPacketHandler>() });
    packetHandlers.insert({ PacketType::AcceptPlayer, std::make_unique<AcceptPlayerPacketHandler>() });
    packetHandlers.insert({ PacketType::RejectPlayer, std::make_unique<RejectPlayerPacketHandler>() });
    packetHandlers.insert({ PacketType::LobbyWelcome, std::make_unique<LobbyWelcomePacketHandler>() });
    packetHandlers.insert({ PacketType::KickPlayer, std::make_unique<KickPlayerPacketHandler>() });
    packetHandlers.insert({ PacketType::StartGame, std::make_unique<StartGamePacketHandler>() });
}

void LobbyState::onLoad()
{
    // Load the initial level
    // TODO: Clients should wait to receive the level name from the host
    ApplicationContext& context = app.getContext();
    const std::string levelName = ConfigUtils::get(context.getConfig(), "levelName", std::string());
    if (levelName.empty())
    {
        throw std::runtime_error("No level name found in config.json ");
    }
    loadLevel(levelName);

    if (host)
    {
        // Add ourselves to the lobby.
        // The host should always have a client ID and player ID of 0.
        ClientInfo localClient(0, localPlayerName);
        onPlayerAccepted(joinRequestId, 0, localClient);
    }
    else
    {
        // We generate a random request ID, just in case 2 players try to join with the same name
        joinRequestId = std::rand();
        RequestJoinPacket joinPacket(joinRequestId, localPlayerName);
        app.getConnection()->send(joinPacket);
    }
}

void LobbyState::update()
{
    pollNetwork();
}

void LobbyState::render(int delta)
{
    menuRenderer.render(delta);
}

void LobbyState::keyUp(const SDL_Keycode keyCode)
{
    switch (keyCode)
    {
    case SDLK_SPACE:
    case SDLK_RETURN:
        // TMP: For now, just start the game when pressing Space or Enter
        if (host)
        {
            requestStartGame();
        }
        break;
    default:
        break;
    }
}

void LobbyState::pollNetwork()
{
    const auto& receivedPackets = app.getConnection()->getReceivedPackets();
    for (auto& packet : receivedPackets)
    {
        auto iter = packetHandlers.find(packet->getType());
        if (iter == packetHandlers.cend())
        {
            std::cerr << "Received unexpected packet of type "
                      << std::to_string(EnumUtils::toIntegral(packet->getType())) << "\n";
            continue;
        }

        iter->second->onPacketReceived(packet, *this);
    }
}

void LobbyState::onPlayerJoinRequest(int requestId, int clientId, const std::string& playerName)
{
    if (!host)
    {
        // Only the host has the authority to accept other players
        return;
    }

    int playerId = requestPlayerId();
    if (playerId >= 0)
    {
        AcceptPlayerPacket acceptPacket(requestId, playerName, playerId);
        app.getConnection()->send(acceptPacket);

        ClientInfo client = { playerId, playerName };
        onPlayerAccepted(requestId, clientId, client);
    }
    else
    {
        RejectPlayerPacket rejectPacket(requestId, playerName);
        app.getConnection()->send(rejectPacket);
        onPlayerRejected(requestId, playerName);
    }
}

void LobbyState::onPlayerAccepted(int requestId, int clientId, const ClientInfo& client)
{
    if (requestId == joinRequestId && client.getName() == localPlayerName)
    {
        // Our request to join was the one that got accepted, which means this is our player ID!
        localPlayerId = client.getPlayerId();
        return;
    }

    std::cout << "Player " << client.getName() << " has joined\n";

    clients.insert({ clientId, client });

    if (host)
    {
        // Inform joining player about the current lobby state
        LobbyWelcomePacket welcomePacket(client.getPlayerId(), clients);
        app.getConnection()->send(welcomePacket);
    }
}

void LobbyState::onPlayerRejected(int requestId, const std::string& playerName)
{
    std::cout << "Player " << playerName << " was not allowed to join\n";

    if (requestId == joinRequestId && playerName == localPlayerName)
    {
        // Our request to join was the one that got rejected
        // TODO: return to main menu
        throw std::runtime_error("Rejected by server");
    }
}

void LobbyState::onWelcomeReceived(int playerId, std::unordered_map<int, ClientInfo> newClients)
{
    if (playerId != localPlayerId)
    {
        // This was not intended for us
        return;
    }

    std::cout << "Received lobby state from host\n";

    clients = newClients;
}

void LobbyState::onPlayerKicked(int playerId)
{
    std::cout << "Player " << std::to_string(playerId) << " was kicked\n";

    if (playerId == localPlayerId)
    {
        // TODO: return to main menu
        throw std::runtime_error("Kicked by server");
    }
}

int LobbyState::requestPlayerId()
{
    // TODO: Find the first available player ID
    // This is a hack! We should consider the max players and and empty slots from disconnected clients.
    int playerId = nextPlayerId;
    ++nextPlayerId;
    return playerId;
}

void LobbyState::loadLevel(const std::string& filename)
{
    ScenarioReader reader(Resources::mapsDir + filename);
    scenarioData = reader.readScenario();
}

void LobbyState::requestStartGame()
{
    if (!host)
    {
        std::cerr << "Non-host player tried to start the game\n";
        return;
    }

    StartGamePacket packet;
    app.getConnection()->send(packet);
    startGame();
}

void LobbyState::startGame()
{
    std::unique_ptr<State> game = createGameState();
    app.setState(std::move(game));
}

std::unique_ptr<State> LobbyState::createGameState() const
{
    ApplicationContext& context = app.getContext();

    // Create the world
    ScenarioBuilder scenarioBuilder(scenarioData);
    EntityFactory entityFactory(context.getResources(), context.getAudioSystem());
    std::unique_ptr<World> world = scenarioBuilder.build(entityFactory);

    // Initialize players
    int numPlayers = PlayerStore::maxPlayers;  // TODO: Read this from the Scenario
    std::unordered_map<int, PlayerState> playerStates;
    for (int playerId = 0; playerId < numPlayers; ++playerId)
    {
        const PlayerProperties& playerProps = scenarioData.playerProperties.at(playerId);
        playerStates.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(playerId),
                std::forward_as_tuple(
                        scenarioBuilder.getRace(playerProps.race),
                        static_cast<int>(playerProps.startingGold),
                        static_cast<int>(playerProps.startingWood),
                        static_cast<int>(playerProps.startingFood)));
    }

    return std::make_unique<GameState>(app, std::move(world), playerStates, clients);
}

}  // namespace Rival
