#include "application/Window.h"

#include "utils/SDLWrapper.h"
#include <SDL_image.h>

#include "application/ResourceLoader.h"
#include "utils/LogUtils.h"

namespace Rival {

Window::Window(int width, int height, const char* title)
    : width(width)
    , height(height)
    , title(title)
    , aspectRatio(static_cast<double>(width) / height)
{
    initSDL();

    sdlWindow = SDL_CreateWindow(title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (sdlWindow == nullptr)
    {
        LOG_ERROR("Failed to create window");
        throw std::runtime_error(SDL_GetError());
    }

    glContext = SDL_GL_CreateContext(sdlWindow);

    if (glContext == nullptr)
    {
        LOG_ERROR("Failed to create OpenGL context");
        throw std::runtime_error(SDL_GetError());
    }

    // Try to enable vsync.
    // Note that vsync may already be enabled by default!
    if (SDL_GL_SetSwapInterval(1) == 0)
    {
        vsyncEnabled = true;
    }
    else
    {
        LOG_WARN("Failed to enable vsync! SDL Error: {}", SDL_GetError());
    }

    // Set the icon
    std::string iconFilename = ResourceLoader::txDir + "icon.png";
    SDL_Surface* surface;
    surface = IMG_Load(iconFilename.c_str());
    setIcon(surface);
    SDL_FreeSurface(surface);
}

Window::~Window()
{
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr;
}

void Window::initSDL()
{
    // Ensure that we only initialize SDL once, in case we create multiple Windows.
    // Really, this initialization should be moved elsewhere.
    static bool isInitialized = false;
    if (isInitialized)
    {
        return;
    }

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

    isInitialized = true;
}

void Window::swapBuffers() const
{
    SDL_GL_SwapWindow(sdlWindow);
}

void Window::setPos(int x, int y)
{
    SDL_SetWindowPosition(sdlWindow, x, y);
}

void Window::setIcon(SDL_Surface* surface)
{
    SDL_SetWindowIcon(sdlWindow, surface);
}

}  // namespace Rival
