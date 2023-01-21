#include "pch.h"

#include "ApplicationContext.h"

#include "SDLWrapper.h"
#include <SDL_image.h>
#include <gl/glew.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "net/NetUtils.h"
#include "AudioUtils.h"
#include "ConfigUtils.h"
#include "FileUtils.h"
#include "GameState.h"
#include "ScenarioBuilder.h"
#include "ScenarioReader.h"
#include "Shaders.h"
#include "World.h"

namespace Rival {

ApplicationContext::ApplicationContext()
    : cfg(readConfig())
    , window((initSDL(), createWindow()))
    , res((initGLEW(), initGL(), initFonts(), *this))
{
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
    return FileUtils::readJsonFile("config.json");
}

void ApplicationContext::initSDL()
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

    // Hide cursor
    SDL_ShowCursor(SDL_DISABLE);
}

void ApplicationContext::initAudio()
{
    AudioUtils::initAL();
    audioSystem.setMidiActive(ConfigUtils::get(cfg, "midiEnabled", true));
    audioSystem.setSoundActive(ConfigUtils::get(cfg, "soundEnabled", true));
}

void ApplicationContext::initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW:" << glewGetErrorString(glewError) << "\n";
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
    std::string iconFilename = Resources::txDir + "icon.png";
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
