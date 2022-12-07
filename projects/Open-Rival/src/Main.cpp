#include "pch.h"

#define SDL_MAIN_HANDLED
#include "SDLWrapper.h"
#include <SDL_image.h>
#include <gl/glew.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>  // std::forward_as_tuple
#include <unordered_map>
#include <utility>  // std::piecewise_construct

#include "Application.h"
#include "AudioUtils.h"
#include "FileUtils.h"
#include "GameState.h"
#include "ScenarioBuilder.h"
#include "ScenarioReader.h"
#include "Shaders.h"
#include "Window.h"
#include "World.h"

using json = nlohmann::json;

using namespace Rival;

json readConfig()
{
    return FileUtils::readJsonFile("config.json");
}

void initSDL()
{
    // This must be called before SDL_Init since we're not using SDL_main
    // as an entry point
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL\n";
        throw std::runtime_error(SDL_GetError());
    }

    // Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW:" << glewGetErrorString(glewError) << "\n";
        throw std::runtime_error("Failed to initialize GLEW");
    }
}

void initGL()
{
    // Set clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Enable back-face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Enable alpha blending
    // See also:
    // https://www.khronos.org/opengl/wiki/Transparency_Sorting#Alpha_test
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shaders::initializeShaders();
}

void initAL()
{
    AudioUtils::initAL();
}

void setWindowIcon(Window& window)
{
    std::string iconFilename = Resources::txDir + "icon.png";
    SDL_Surface* surface;
    surface = IMG_Load(iconFilename.c_str());
    window.setIcon(surface);
    SDL_FreeSurface(surface);
}

std::unique_ptr<Window> createWindow()
{
    std::unique_ptr<Window> window = std::make_unique<Window>(800, 600, "Rival Realms");
    setWindowIcon(*window);
    return window;
}

void exit()
{
    AudioUtils::destroyAL();
    SDL_Quit();
}

/**
 * Entry point for the application.
 */
int main()
{
    int exitCode = 0;

    try
    {
        // Initialization that does not require an OpenGL context
        json cfg = readConfig();
        initSDL();
        initAL();

        std::unique_ptr<Window> window = createWindow();

        // Initialization that requires an OpenGL context
        initGLEW();
        initGL();

        // Create our Application
        Application app(*window, cfg);

        // Load some scenario
        ScenarioReader reader(Resources::mapsDir + "test_pathfinding.sco");
        ScenarioBuilder scenarioBuilder(reader.readScenario());
        Resources& res = app.getResources();
        EntityFactory entityFactory(res, app.getAudioSystem());
        std::unique_ptr<World> world = scenarioBuilder.build(entityFactory);

        // Initialize players
        std::unordered_map<int, PlayerState> playerStates;
        playerStates.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(0),
                std::forward_as_tuple(Race::Human, /* gold = */ 100, /* wood = */ 100, /* food = */ 100));

        // Create our initial state
        std::unique_ptr<State> initialState = std::make_unique<GameState>(app, std::move(world), playerStates);

        // Run the game!
        app.start(std::move(initialState));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unhandled error during initialization or gameplay\n";
        std::cerr << e.what() << "\n";
        exitCode = 1;
    }

    exit();

    return exitCode;
}
