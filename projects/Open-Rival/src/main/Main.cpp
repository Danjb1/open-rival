#include "utils/SDLWrapper.h"  // Needed for SDL_MAIN_HANDLED
#include <spdlog/spdlog.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>  // std::forward_as_tuple
#include <unordered_map>
#include <utility>  // std::move, std::piecewise_construct

#include "application/Application.h"
#include "application/Window.h"
#include "gfx/opengl/GLRenderer.h"
#include "lobby/LobbyState.h"
#include "main/ProgramOptions.h"
#include "net/NetUtils.h"
#include "utils/ConfigUtils.h"
#include "utils/LogUtils.h"

using namespace Rival;

void initLogging(const json& cfg)
{
    const std::string defaultLogLevel = "info";
    const std::string logLevel = ConfigUtils::get(cfg, "logLevel", defaultLogLevel);
    const bool logToConsole = ConfigUtils::get(cfg, "logToConsole", false);
    const bool logToFile = ConfigUtils::get(cfg, "logToFile", true);
    LogUtils::initLogging(logLevel, logToConsole, logToFile, "Open-Rival");

    // Categories
    LogUtils::makeLogCategory("attack", ConfigUtils::get(cfg, "logLevelAttack", defaultLogLevel));
    LogUtils::makeLogCategory("movement", ConfigUtils::get(cfg, "logLevelMovement", defaultLogLevel));
    LogUtils::makeLogCategory("pathfinding", ConfigUtils::get(cfg, "logLevelPathfinding", defaultLogLevel));
}

/*
 * Call our normal main function from WinMain.
 * WinMain is used when building with the WIN32_EXECUTABLE flag.
 */
#ifdef _WIN32
#include <windows.h>

extern int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return main(__argc, __argv);
}
#endif

/**
 * Entry point for the application.
 */
int main(int argc, char* argv[])
{
    // Parse command-line parameters
    ProgramOptions options(argc, argv);
    if (options.hasError())
    {
        LOG_ERROR("{}", options.getError());
        return -1;
    }

    int exitCode = 0;
    FT_Library fontLib = nullptr;

#if !DEBUG
    // Fail gracefully if we are not debugging
    try
    {
#endif

        // Read the config file first, since this determines our logging capabilities
        json cfg = JsonUtils::readJsonFile("config.json");

        // Initialize logging nice and early
        initLogging(cfg);

        // Initialize engine subsystems
        // --- Window ---
        Window window(800, 600, "Rival Realms");
        if (options.hasWindowPosition())
        {
            auto windowPos = options.getWindowPos();
            window.setPos(windowPos.x, windowPos.y);
        }

        // --- Rendering ---
        GLRenderer renderer(&window);

        // --- Fonts ---
        if (FT_Error err = FT_Init_FreeType(&fontLib))
        {
            throw std::runtime_error("Failed to initialize FreeType library: " + std::to_string(err));
        }

        // Load resources
        Resources res;
        ResourceLoader(cfg, &renderer, fontLib, res);

        // --- Audio ---
        AudioSystem audioSystem;
        audioSystem.setMidiActive(ConfigUtils::get(cfg, "midiEnabled", true));
        audioSystem.setSoundActive(ConfigUtils::get(cfg, "soundEnabled", true));
        audioSystem.prepareSounds(res.getAllSounds());

        NetUtils::initNetworking();

        // Create the application
        Application app(cfg, &window, &renderer, audioSystem, res, fontLib);

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

#if !DEBUG
    }
    // Fail gracefully if we are not debugging
    catch (const std::exception& e)
    {
        LOG_ERROR("Unhandled error during initialization or gameplay: {}", e.what());
        exitCode = 1;
    }
#endif

    LOG_DEBUG("Performing clean-up...");
    NetUtils::destroyNetworking();
    FT_Done_FreeType(fontLib);
    SDL_Quit();

    return exitCode;
}
