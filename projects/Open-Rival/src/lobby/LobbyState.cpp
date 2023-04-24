#include "pch.h"

#include "lobby/LobbyState.h"

#include "Application.h"
#include "ApplicationContext.h"
#include "ConfigUtils.h"
#include "GameState.h"
#include "PlayerState.h"
#include "ScenarioBuilder.h"
#include "ScenarioReader.h"
#include "World.h"

namespace Rival {

LobbyState::LobbyState(Application& app)
    : State(app)
{
}

void LobbyState::onLoad()
{
    // TMP
    startGame();
}

void LobbyState::update()
{
    // TODO
}

void LobbyState::render(int /*delta*/)
{
    // TODO
}

void LobbyState::startGame()
{
    std::unique_ptr<State> game = createGameState();
    app.setState(std::move(game));
}

std::unique_ptr<State> LobbyState::createGameState() const
{
    ApplicationContext& context = app.getContext();

    // Verify that we have a valid level
    const std::string levelName = ConfigUtils::get(context.getConfig(), "levelName", std::string());
    if (levelName.empty())
    {
        throw std::runtime_error("No level name found in config.json ");
    }
    ScenarioReader reader(Resources::mapsDir + levelName);

    // Load the scenario
    ScenarioBuilder scenarioBuilder(reader.readScenario());
    EntityFactory entityFactory(context.getResources(), context.getAudioSystem());
    std::unique_ptr<World> world = scenarioBuilder.build(entityFactory);

    // Initialize players
    // TODO: Read this from the Scenario
    std::unordered_map<int, PlayerState> playerStates;
    playerStates.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(0),
            std::forward_as_tuple(Race::Human, /* gold = */ 100, /* wood = */ 100, /* food = */ 100));

    return std::make_unique<GameState>(app, std::move(world), playerStates);
}

}  // namespace Rival
