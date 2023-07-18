#include "application/Window.h"

#include "utils/SDLWrapper.h"

#include "utils/LogUtils.h"

namespace Rival {

Window::Window(int width, int height, const char* title)
    : width(width)
    , height(height)
    , title(title)
    , aspectRatio(static_cast<double>(width) / height)
{

    sdlWindow = SDL_CreateWindow(
            title,
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
}

Window::~Window()
{
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr;
}

void Window::swapBuffers() const
{
    SDL_GL_SwapWindow(sdlWindow);
}

void Window::setIcon(SDL_Surface* surface)
{
    SDL_SetWindowIcon(sdlWindow, surface);
}

}  // namespace Rival
