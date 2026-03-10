#include "game/GameState.h"

#include <map>  // std::cend
#include <stdexcept>
#include <string>
#include <utility>  // std::move

#include "application/Application.h"
#include "entity/components/HealthComponent.h"  // TMP
#include "entity/components/OwnerComponent.h"
#include "game/GameInterface.h"
#include "game/Race.h"
#include "gfx/Image.h"
#include "gfx/Palette.h"
#include "gfx/RenderUtils.h"
#include "gfx/Spritesheet.h"
#include "gfx/opengl/renderer/MenuTextRenderer.h"  // TMP
#include "net/Connection.h"
#include "net/packet-handlers/GameCommandPacketHandler.h"
#include "net/packet-handlers/PacketHandler.h"
#include "net/packets/GameCommandPacket.h"
#include "utils/EnumUtils.h"
#include "utils/InputUtils.h"
#include "utils/LogUtils.h"
#include "utils/MouseUtils.h"
#include "utils/TimeUtils.h"

namespace Rival {

GameState::GameState(Application& app,
        std::unique_ptr<World> scenarioToMove,
        std::unordered_map<int, PlayerState> playerStates,
        std::unordered_map<int, ClientInfo> clients,
        int localPlayerId,
        std::shared_ptr<std::mt19937> randomizer)
    : State(app)
    , world(std::move(scenarioToMove))
    , playerStates(playerStates)
    , viewport(0, 0, window->getWidth(), window->getHeight() - GameInterface::uiHeight)
    , camera(0.0f,
              0.0f,
              RenderUtils::pxToCamera_X(static_cast<float>(viewport.width)),
              RenderUtils::pxToCamera_Y(static_cast<float>(viewport.height)),
              *world)
    , playerContext()
    , mousePicker(std::make_shared<MousePicker>(camera, viewport, *world, playerContext, *this, *this))
    , clients(clients)
    , localPlayerId(localPlayerId)
    , randomizer(randomizer)
{
    // Register PacketHandlers
    packetHandlers.insert({ PacketType::GameCommand, std::make_unique<GameCommandPacketHandler>() });
}

void GameState::onLoad()
{
    app.getRenderer()->onEnterGame(this);
    app.getAudioSystem().playMidi(res.getMidi(0));
}

void GameState::update(int delta)
{
    pollNetwork();
    if (!isTickReady())
    {
        bIsWaitingForPlayers = true;
        return;
    }

    bIsWaitingForPlayers = false;
    world->addPendingEntities();
    earlyUpdateEntities();
    respondToInput();
    sendOutgoingCommands();
    updateEntities(delta);
    processCommands();
    ++currentTick;
}

bool GameState::isTickReady() const
{
    if (!isNetGame())
    {
        return true;
    }

    if (currentTick < TimeUtils::netCommandDelay)
    {
        // No commands should be scheduled before this time
        return true;
    }

    const auto iter = clientsReady.find(currentTick);
    if (iter == clientsReady.cend())
    {
        // No clients are ready! We can still process the next tick if we are the only player (unlikely for a net game,
        // but technically possible).
        return clients.empty();
    }

    const std::unordered_set<int>& clientsReadyForTick = iter->second;
    const std::size_t numClientsReady = clientsReadyForTick.size();

    return numClientsReady == clients.size();
}

void GameState::pollNetwork()
{
    if (!isNetGame())
    {
        return;
    }

    const auto& receivedPackets = app.getConnection()->getReceivedPackets();
    for (auto& packet : receivedPackets)
    {
        const auto iter = packetHandlers.find(packet->getType());
        if (iter == packetHandlers.cend())
        {
            LOG_WARN("Received unexpected packet of type {}", EnumUtils::toIntegral(packet->getType()));
            continue;
        }

        iter->second->onPacketReceived(packet, *this);
    }
}

void GameState::earlyUpdateEntities() const
{
    world->forEachMutableEntity([](const auto& entity) { entity->earlyUpdate(); });
}

void GameState::respondToInput()
{
    mousePicker->handleMouse();

    // Move camera
    if (input.lastDirectionX == Direction::Decreasing)
    {
        camera.translate(-0.5f, 0.f);
    }
    else if (input.lastDirectionX == Direction::Increasing)
    {
        camera.translate(0.5f, 0.f);
    }
    if (input.lastDirectionY == Direction::Decreasing)
    {
        camera.translate(0.f, -0.35f);
    }
    else if (input.lastDirectionY == Direction::Increasing)
    {
        camera.translate(0.f, 0.35f);
    }
}

void GameState::sendOutgoingCommands()
{
    if (!isNetGame())
    {
        return;
    }

    if (!app.getConnection()->isOpen())
    {
        // Don't try to send commands if the connection is closed.
        // Later, we could display a message and/or try to reconnect.
        return;
    }

    // Send all commands for this tick to the server
    auto packet = std::make_shared<GameCommandPacket>(outgoingCommands, currentTick + TimeUtils::netCommandDelay);
    app.getConnection()->send(packet);
    outgoingCommands.clear();
}

void GameState::updateEntities(int delta) const
{
    std::vector<std::shared_ptr<Entity>> deletedEntities;

    // Update entities and check for deletions
    world->forEachMutableEntity([&](const auto& entity) {
        entity->update(delta);
        if (entity->isDeleted())
        {
            deletedEntities.push_back(entity);
        }
    });

    // Remove deleted Entities
    for (const auto& e : deletedEntities)
    {
        world->removeEntity(e);
        e->removedFromWorld();
        e->destroy();
    }
}

void GameState::render(int delta)
{
    Renderer* renderer = app.getRenderer();
    renderer->renderGame(delta);
}

void GameState::keyDown(const SDL_Keycode keyCode)
{
    switch (keyCode)
    {
    case SDLK_UP:
        input.lastDirectionY = Direction::Decreasing;
        break;

    case SDLK_DOWN:
        input.lastDirectionY = Direction::Increasing;
        break;

    case SDLK_LEFT:
        input.lastDirectionX = Direction::Decreasing;
        break;

    case SDLK_RIGHT:
        input.lastDirectionX = Direction::Increasing;
        break;

    case SDLK_DELETE: {
        // TMP: Kill selected unit
        auto weakSelectedEntity = playerContext.getFirstSelectedEntity();
        if (auto selectedEntity = weakSelectedEntity.lock())
        {
            if (HealthComponent* health = selectedEntity->getComponent<HealthComponent>())
            {
                health->addHealth(-9999);
            }
        }
        break;
    }

    default:
        break;
    }
}

void GameState::keyUp(const SDL_Keycode keyCode)
{
    switch (keyCode)
    {
    case SDLK_UP:
        input.lastDirectionY = InputUtils::isKeyDown(SDLK_DOWN) ? Direction::Decreasing : Direction::None;
        break;

    case SDLK_DOWN:
        input.lastDirectionY = InputUtils::isKeyDown(SDLK_UP) ? Direction::Increasing : Direction::None;
        break;

    case SDLK_LEFT:
        input.lastDirectionX = InputUtils::isKeyDown(SDLK_RIGHT) ? Direction::Decreasing : Direction::None;
        break;

    case SDLK_RIGHT:
        input.lastDirectionX = InputUtils::isKeyDown(SDLK_LEFT) ? Direction::Increasing : Direction::None;
        break;

    case SDLK_a:
        requestAttackMode();
        break;

    default:
        break;
    }
}

void GameState::requestAttackMode()
{
    std::shared_ptr<Entity> selectedEntity = playerContext.getFirstSelectedEntity().lock();
    const auto selectedOwnerComp = selectedEntity ? selectedEntity->getComponent<OwnerComponent>() : nullptr;
    const bool isOwnUnitSelected = selectedOwnerComp && isLocalPlayer(selectedOwnerComp->getPlayerId());
    if (isOwnUnitSelected)
    {
        playerContext.setCurrentAction(PlayerAction::Attack);
    }
}

void GameState::mouseDown(const SDL_MouseButtonEvent evt)
{
    mousePicker->mouseDown(evt.button);
}

void GameState::mouseUp(const SDL_MouseButtonEvent evt)
{
    mousePicker->mouseUp(evt.button);
}

void GameState::mouseWheelMoved(const SDL_MouseWheelEvent evt)
{
    // Get the mouse position relative to the window, in pixels
    int mouseInViewportX;
    int mouseInViewportY;
    SDL_GetMouseState(&mouseInViewportX, &mouseInViewportY);

    // Abort if the mouse is outside the viewport
    if (!viewport.contains(mouseInViewportX, mouseInViewportY))
    {
        return;
    }

    // Check if any scrolling took place
    int scrollDelta = evt.y;
    if (scrollDelta == 0)
    {
        return;
    }

    // Determine the scroll direction
    if (evt.direction == SDL_MOUSEWHEEL_FLIPPED)
    {
        scrollDelta *= -1;
    }

    // Find the mouse position in scaled pixels before the zoom
    float cameraX_px = RenderUtils::cameraToPx_X(camera.getLeft());
    float cameraY_px = RenderUtils::cameraToPx_Y(camera.getTop());
    const float zoomBefore = camera.getZoom();
    const float mouseXBefore = cameraX_px + (mouseInViewportX / zoomBefore);
    const float mouseYBefore = cameraY_px + (mouseInViewportY / zoomBefore);

    // Zoom
    camera.applyZoom(static_cast<float>(scrollDelta));

    // If the zoom level hasn't changed, there is no need to translate the camera
    const float zoomAfter = camera.getZoom();
    if (zoomBefore == zoomAfter)
    {
        return;
    }

    // Find the mouse position in scaled pixels after the zoom
    cameraX_px = RenderUtils::cameraToPx_X(camera.getLeft());
    cameraY_px = RenderUtils::cameraToPx_Y(camera.getTop());
    float mouseXAfter = cameraX_px + (mouseInViewportX / zoomAfter);
    float mouseYAfter = cameraY_px + (mouseInViewportY / zoomAfter);

    // Determine how much the mouse has moved in scaled pixels
    const float mouseDx = mouseXBefore - mouseXAfter;
    const float mouseDy = mouseYBefore - mouseYAfter;

    // Convert back to camera co-ordinates
    const float mouseDxCamera = RenderUtils::pxToCamera_X(mouseDx);
    const float mouseDyCamera = RenderUtils::pxToCamera_Y(mouseDy);

    // Translate the camera to keep the mouse at the same world co-ordinates
    camera.translate(mouseDxCamera, mouseDyCamera);
}

World& GameState::getWorld()
{
    return *world;
}

const PlayerState& GameState::getLocalPlayerState() const
{
    const auto result = playerStates.find(localPlayerId);
    if (result == playerStates.cend())
    {
        throw std::runtime_error("No local player state found!");
    }

    return result->second;
}

const PlayerState* GameState::getPlayerState(int playerId) const
{
    const auto result = playerStates.find(playerId);
    return result == playerStates.cend() ? nullptr : &result->second;
}

bool GameState::isLocalPlayer(int playerId) const
{
    return playerId == localPlayerId;
}

bool GameState::isSameTeam(int playerId) const
{
    // TODO: Support teams
    return playerId == localPlayerId;
}

void GameState::dispatchCommand(std::shared_ptr<GameCommand> command)
{
    if (!command)
    {
        LOG_WARN("Tried to dispatch invalid command");
        return;
    }

    // In multiplayer, schedule commands for 'n' ticks in the future
    int relevantTick = isNetGame() ? currentTick + TimeUtils::netCommandDelay : currentTick;
    scheduleCommand(command, relevantTick);

    // Queue commands to be sent over the network
    if (isNetGame())
    {
        outgoingCommands.push_back(command);
    }
}

void GameState::scheduleCommand(std::shared_ptr<GameCommand> command, int tick)
{
    const auto findResult = pendingCommands.find(tick);
    if (findResult == pendingCommands.cend())
    {
        // No commands are scheduled for this tick yet
        pendingCommands.insert({ tick, { command } });
    }
    else
    {
        // Add our command to the commands already scheduled for this tick
        std::vector<std::shared_ptr<GameCommand>>& commandsDue = findResult->second;
        commandsDue.push_back(command);
    }
}

void GameState::onClientReady(int tick, int clientId)
{
    // Ensure `clientsReady` has an entry for the tick in question
    auto [iter, inserted] = clientsReady.try_emplace(tick);
    auto& clientsReadyForTick = iter->second;

    // Add the given client to the ready list
    auto [_, added] = clientsReadyForTick.insert(clientId);
    if (!added)
    {
        throw std::runtime_error("Duplicate player ready received for client " + std::to_string(clientId)
                + " for tick: " + std::to_string(tick));
    }
}

bool GameState::isWaitingForPlayers() const
{
    return bIsWaitingForPlayers;
}

std::vector<ClientInfo> GameState::getNotReadyClients() const
{
    const auto iter = clientsReady.find(currentTick);
    const bool areAnyClientsReady = iter != clientsReady.cend();
    const std::unordered_set<int> clientsReadyThisTick = areAnyClientsReady ? iter->second : std::unordered_set<int>();

    std::vector<ClientInfo> notReadyClients;

    for (const auto& client : clients)
    {
        if (!clientsReadyThisTick.contains(client.first))
        {
            notReadyClients.push_back(client.second);
        }
    }

    return notReadyClients;
}

const Camera& GameState::getCamera() const
{
    return camera;
}

const Rect& GameState::getViewport() const
{
    return viewport;
}

const PlayerContext& GameState::getPlayerContext() const
{
    return playerContext;
}

void GameState::processCommands()
{
    const auto findResult = pendingCommands.find(currentTick);
    if (findResult == pendingCommands.cend())
    {
        // No commands due
        return;
    }

    std::vector<std::shared_ptr<GameCommand>>& commandsDue = findResult->second;
    for (auto& cmd : commandsDue)
    {
        cmd->execute(*this);
    }

    pendingCommands.erase(currentTick);
    clientsReady.erase(currentTick);
}

bool GameState::isNetGame() const
{
    return app.getConnection().has_value();
}

int GameState::getNumPlayers() const
{
    return static_cast<int>(playerStates.size());
}

}  // namespace Rival
