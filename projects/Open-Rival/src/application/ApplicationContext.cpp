#include "application/ApplicationContext.h"

#include "utils/SDLWrapper.h"
#include <SDL_image.h>
#include "gfx/GlewWrapper.h"

#include <chrono>
#include <format>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>  // std::move

#include "application/ResourceLoader.h"
#include "audio/AudioUtils.h"
#include "game/GameState.h"
#include "game/World.h"
#include "gfx/Shaders.h"
#include "net/NetUtils.h"
#include "scenario/ScenarioBuilder.h"
#include "scenario/ScenarioReader.h"
#include "utils/ConfigUtils.h"
#include "utils/JsonUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

ApplicationContext::ApplicationContext()
    /* We initialize logging as early as possible, but we need to read the config first to know the log level. */
    : cfg(readConfig())
    , window((initLogging(), initSDL(), createWindow()))
{
    initGLEW();
    initGL();
    initFonts();

    ResourceLoader(*this, res);  // Load resources

    initAudio();
    NetUtils::initNetworking();
}

ApplicationContext::~ApplicationContext()
{
    NetUtils::destroyNetworking();
    FT_Done_FreeType(fontLib);
    AudioUtils::destroyAL();
    SDL_Quit();
}

json ApplicationContext::readConfig()
{
    return JsonUtils::readJsonFile("config.json");
}

void ApplicationContext::initLogging()
{
    const std::string defaultLogLevel = "info";
    const std::string logLevel = ConfigUtils::get(cfg, "logLevel", defaultLogLevel);
    const bool logToConsole = ConfigUtils::get(cfg, "logToConsole", false);
    const bool logToFile = ConfigUtils::get(cfg, "logToFile", true);
    LogUtils::initLogging(logLevel, logToConsole, logToFile, "Open-Rival");

    // Categories
    LogUtils::makeLogCategory("pathfinding", ConfigUtils::get(cfg, "logLevelPathfinding", defaultLogLevel));
}

void ApplicationContext::initSDL()
{
    LOG_DEBUG("Initializing...");

    // This must be called before SDL_Init since we're not using SDL_main
    // as an entry point
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG_ERROR("Failed to initialize SDL");
        throw std::runtime_error(SDL_GetError());
    }

    // Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Hide cursor
    SDL_ShowCursor(SDL_DISABLE);
}

void ApplicationContext::initAudio()
{
    AudioUtils::initAL();
    audioSystem.setMidiActive(ConfigUtils::get(cfg, "midiEnabled", true));
    audioSystem.setSoundActive(ConfigUtils::get(cfg, "soundEnabled", true));
    audioSystem.prepareSounds(res.getAllSounds());
}

void ApplicationContext::initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        const GLubyte* glewErrorString = glewGetErrorString(glewError);
        std::string errorString(reinterpret_cast<const char*>(glewErrorString));
        LOG_ERROR("Error initializing GLEW: {}", errorString);
        throw std::runtime_error("Failed to initialize GLEW");
    }
}

void ApplicationContext::initGL()
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

std::unique_ptr<Window> ApplicationContext::createWindow()
{
    std::unique_ptr<Window> window = std::make_unique<Window>(800, 600, "Rival Realms");
    setWindowIcon(*window);
    return std::move(window);
}

void ApplicationContext::setWindowIcon(Window& window)
{
    std::string iconFilename = ResourceLoader::txDir + "icon.png";
    SDL_Surface* surface;
    surface = IMG_Load(iconFilename.c_str());
    window.setIcon(surface);
    SDL_FreeSurface(surface);
}

void ApplicationContext::initFonts()
{
    if (FT_Error err = FT_Init_FreeType(&fontLib))
    {
        throw std::runtime_error("Failed to initialize FreeType library: " + std::to_string(err));
    }
}

}  // namespace Rival
