#include "lobby/LobbyState.h"

#include <chrono>
#include <random>

#include "application/Application.h"
#include "application/ResourceLoader.h"
#include "entity/EntityFactory.h"
#include "game/GameState.h"
#include "game/PlayerState.h"
#include "game/World.h"
#include "gfx/Renderer.h"
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
#include "scenario/ScenarioBuilder.h"
#include "scenario/ScenarioReader.h"
#include "utils/ConfigUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

LobbyState::LobbyState(Application& app, std::string playerName, bool bIsHost)
    : State(app)
    , bIsHost(bIsHost)
    , localPlayerName(playerName)
{
    // Register PacketHandlers
    packetHandlers.insert({ PacketType::RequestJoin, std::make_unique<RequestJoinPacketHandler>() });
    packetHandlers.insert({ PacketType::AcceptPlayer, std::make_unique<AcceptPlayerPacketHandler>() });
    packetHandlers.insert({ PacketType::RejectPlayer, std::make_unique<RejectPlayerPacketHandler>() });
    packetHandlers.insert({ PacketType::LobbyWelcome, std::make_unique<LobbyWelcomePacketHandler>() });
    packetHandlers.insert({ PacketType::KickPlayer, std::make_unique<KickPlayerPacketHandler>() });
    packetHandlers.insert({ PacketType::StartGame, std::make_unique<StartGamePacketHandler>() });

    if (bIsHost)
    {
        // Determine the seed that we will use for all our random numbers once the game starts.
        // It is imperative that all players generate the same sequence of random numbers.
        randomSeed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    }
}

static int generateJoinRequestId()
{
    // This only matters if 2 players try to join with the same name.
    // In this case, we need to come up with unique identifiers to differentiate them.
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> dist;
    return dist(rng);
}

void LobbyState::onLoad()
{
    app.getRenderer()->onEnterLobby(this);

    // Load the initial level
    // TODO: Clients should wait to receive the level name from the host
    const std::string levelName = ConfigUtils::get(app.getConfig(), "levelName", std::string());
    if (levelName.empty())
    {
        throw std::runtime_error("No level name found in config.json ");
    }
    loadLevel(levelName);

    if (isNetGame())
    {
        if (bIsHost)
        {
            // Add ourselves to the lobby.
            // The host should always have a client ID and player ID of 0.
            ClientInfo localClient(0, localPlayerName);
            onPlayerAccepted(joinRequestId, 0, localClient);
        }
        else
        {
            joinRequestId = generateJoinRequestId();
            auto joinPacket = std::make_shared<RequestJoinPacket>(joinRequestId, localPlayerName);
            app.getConnection()->send(joinPacket);
        }
    }
    else
    {
        // Just start the game immediately
        localPlayerId = 0;
        startGame();
    }
}

void LobbyState::update(int /* delta */)
{
    pollNetwork();
}

void LobbyState::render(int delta)
{
    Renderer* renderer = app.getRenderer();
    renderer->renderLobby(delta);
}

void LobbyState::keyUp(const SDL_Keycode keyCode)
{
    switch (keyCode)
    {
    case SDLK_SPACE:
    case SDLK_RETURN:
        // TMP: For now, just start the game when pressing Space or Enter
        if (bIsHost)
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
    if (!isNetGame())
    {
        return;
    }

    auto& connection = app.getConnection();
    const auto& receivedPackets = connection->getReceivedPackets();
    for (int i = 0; i < receivedPackets.size(); ++i)
    {
        const auto& packet = receivedPackets[i];
        const auto iter = packetHandlers.find(packet->getType());
        if (iter == packetHandlers.cend())
        {
            LOG_WARN("Received unexpected packet of type {}", EnumUtils::toIntegral(packet->getType()));
            continue;
        }

        iter->second->onPacketReceived(packet, *this);

        if (isGameStarted)
        {
            // Any packets received after this point are intended for the next state
            std::vector<std::shared_ptr<const Packet>> packetsToReturn(
                    receivedPackets.cbegin() + i + 1, receivedPackets.cend());
            connection->returnPackets(packetsToReturn);
            break;
        }
    }
}

void LobbyState::onPlayerJoinRequest(int requestId, int clientId, const std::string& playerName)
{
    if (!bIsHost)
    {
        // Only the host has the authority to accept other players
        return;
    }

    int playerId = requestPlayerId();
    if (playerId >= 0)
    {
        LOG_INFO("Assigned player ID {} to {}", playerId, playerName);

        auto acceptPacket = std::make_shared<AcceptPlayerPacket>(requestId, clientId, playerName, playerId);
        app.getConnection()->send(acceptPacket);

        ClientInfo client = { playerId, playerName };
        onPlayerAccepted(requestId, clientId, client);
    }
    else
    {
        auto rejectPacket = std::make_shared<RejectPlayerPacket>(requestId, playerName);
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
        LOG_INFO("Assigned player ID: {}", localPlayerId);
        return;
    }

    LOG_INFO("Player {} has joined the lobby (client {})", client.getName(), clientId);

    if (bIsHost)
    {
        // Inform joining player about the current lobby state
        std::unordered_map<int, ClientInfo> clientsIncludingHost = clients;
        ClientInfo localClient(0, localPlayerName);
        clientsIncludingHost.insert({ 0, localClient });
        auto welcomePacket =
                std::make_shared<LobbyWelcomePacket>(client.getPlayerId(), clientsIncludingHost, randomSeed);
        app.getConnection()->send(welcomePacket);
    }

    clients.insert({ clientId, client });
}

void LobbyState::onPlayerRejected(int requestId, const std::string& playerName)
{
    LOG_INFO("Player {} was not allowed to join", playerName);

    if (requestId == joinRequestId && playerName == localPlayerName)
    {
        // Our request to join was the one that got rejected
        // TODO: return to main menu
        throw std::runtime_error("Rejected by server");
    }
}

void LobbyState::onWelcomeReceived(
        int playerId, std::unordered_map<int, ClientInfo> newClients, unsigned int newRandomSeed)
{
    if (playerId != localPlayerId)
    {
        // This was not intended for us
        return;
    }

    LOG_INFO("Received lobby state from host");

    clients = newClients;
    randomSeed = newRandomSeed;
}

void LobbyState::onPlayerKicked(int playerId)
{
    LOG_INFO("Player {} was kicked", std::to_string(playerId));

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
    ScenarioReader reader(ResourceLoader::mapsDir + filename);
    scenarioData = reader.readScenario();
}

void LobbyState::requestStartGame()
{
    if (!bIsHost)
    {
        LOG_WARN("Non-host player tried to start the game");
        return;
    }

    auto startGamePacket = std::make_shared<StartGamePacket>();
    app.getConnection()->send(startGamePacket);
    startGame();
}

void LobbyState::startGame()
{
    LOG_INFO("Starting game!");

    LOG_DEBUG("Connected clients:");
    for (const auto& client : clients)
    {
        LOG_DEBUG("  Client {} -> Player {} ({})", client.first, client.second.getPlayerId(), client.second.getName());
    }

    std::unique_ptr<State> game = createGameState();
    app.setState(std::move(game));
    isGameStarted = true;
}

std::unique_ptr<State> LobbyState::createGameState() const
{
    LOG_INFO("Using random seed: {}", randomSeed);
    std::shared_ptr<std::mt19937> randomizer = std::make_shared<std::mt19937>(randomSeed);

    // Create the world
    ScenarioBuilder scenarioBuilder(scenarioData);
    std::shared_ptr<const EntityFactory> entityFactory =
            std::make_shared<EntityFactory>(res, app.getAudioSystem(), randomizer);
    std::unique_ptr<World> world = scenarioBuilder.build(entityFactory);

    // Initialize players
    int numPlayers = PlayerStore::maxPlayers;  // TODO: Read this from the Scenario
    std::unordered_map<int, PlayerState> playerStates;
    for (int playerId = 0; playerId < numPlayers; ++playerId)
    {
        const PlayerProperties& playerProps = scenarioData.playerProperties.at(playerId);
        playerStates.emplace(std::piecewise_construct,
                std::forward_as_tuple(playerId),
                std::forward_as_tuple(scenarioBuilder.getRace(playerProps.race),
                        static_cast<int>(playerProps.startingGold),
                        static_cast<int>(playerProps.startingWood),
                        static_cast<int>(playerProps.startingFood)));
    }

    return std::make_unique<GameState>(app, std::move(world), playerStates, clients, localPlayerId, randomizer);
}

bool LobbyState::isNetGame() const
{
    return app.getConnection().has_value();
}

bool LobbyState::isHost() const
{
    return bIsHost;
}

std::string LobbyState::getLocalPlayerName() const
{
    return localPlayerName;
}

std::unordered_map<int, ClientInfo> LobbyState::getClients() const
{
    return clients;
}

}  // namespace Rival
