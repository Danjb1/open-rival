#include <spdlog/spdlog.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>  // std::forward_as_tuple
#include <unordered_map>
#include <utility>  // std::move, std::piecewise_construct

#include "application/Application.h"
#include "application/ApplicationContext.h"
#include "lobby/LobbyState.h"
#include "main/ProgramOptions.h"

using namespace Rival;

/**
 * Entry point for the application.
 */
int main(int argc, char* argv[])
{
    spdlog::info("Test log!");

    // Parse command-line parameters
    ProgramOptions options(argc, argv);
    if (options.hasError())
    {
        std::cerr << options.getError() << "\n";
        return -1;
    }

    int exitCode = 0;

    try
    {
        ApplicationContext context;
        Application app(context);

        // Host or join a game;
        // eventually this will be handled before we enter the LobbyState
        if (options.isHost())
        {
            app.startServer(options.getPort());
        }
        else if (options.isClient())
        {
            app.connectToServer(options.getHostAddress(), options.getPort());
        }

        bool hostForLobby = options.isNetworked() ? options.isHost() : true;
        std::string playerName = hostForLobby ? "Host" : "Client";
        std::unique_ptr<State> initialState = std::make_unique<LobbyState>(app, playerName, hostForLobby);
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
