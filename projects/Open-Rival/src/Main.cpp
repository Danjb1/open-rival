#include "pch.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>  // std::forward_as_tuple
#include <unordered_map>
#include <utility>  // std::piecewise_construct

#include "Application.h"
#include "ApplicationContext.h"
#include "ConfigUtils.h"
#include "GameState.h"
#include "PlayerState.h"
#include "ScenarioBuilder.h"
#include "ScenarioReader.h"
#include "World.h"

using namespace Rival;

std::unique_ptr<State> createGameState(Application& app)
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

/**
 * Entry point for the application.
 */
int main()
{
    int exitCode = 0;

    try
    {
        ApplicationContext context;
        Application app(context);

        // Host a game
        // TMP: These variables should be read from the command line
        bool host = true;
        const std::string address = "localhost";
        bool join = false;
        int port = 28039;
        if (host)
        {
            app.startServer(port);
        }
        else if (join)
        {
            app.connectToServer(address, port);
        }

        std::unique_ptr<State> initialState = createGameState(app);
        app.start(std::move(initialState));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unhandled error during initialization or gameplay\n";
        std::cerr << e.what() << "\n";
        exitCode = 1;
    }

    return exitCode;
}
