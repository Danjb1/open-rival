#include "application/Window.h"

#include "utils/SDLWrapper.h"

#include <iostream>

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
        std::cerr << "Window could not be created!\n";
        throw std::runtime_error(SDL_GetError());
    }

    glContext = SDL_GL_CreateContext(sdlWindow);

    if (glContext == nullptr)
    {
        std::cerr << "OpenGL context could not be created!\n";
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
        std::cerr << "Unable to enable vsync! SDL Error: " << SDL_GetError() << "\n";
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
