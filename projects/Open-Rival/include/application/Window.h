#pragma once

#include "utils/SDLWrapper.h"

#include <string>
#include <vector>

namespace Rival {

/** A window created with SDL.
 * Later, we should make this abstract and create a concrete SDLWindow implementation. */
class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    // Disable moving / copying
    Window(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window& operator=(const Window& other) = delete;
    Window& operator=(Window&& other) = delete;

    void swapBuffers() const;

    void setIcon(SDL_Surface* surface);

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    double getAspectRatio() const
    {
        return aspectRatio;
    }

    bool isVsyncEnabled() const
    {
        return vsyncEnabled;
    }

private:
    static void initSDL();

private:
    SDL_Window* sdlWindow = nullptr;
    SDL_GLContext glContext = nullptr;

    const int width;
    const int height;
    const double aspectRatio;
    const char* title;
    bool vsyncEnabled { false };
};

}  // namespace Rival
