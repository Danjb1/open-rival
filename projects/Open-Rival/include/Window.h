#pragma once

#include "SDLWrapper.h"

#include <string>
#include <vector>

namespace Rival {

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
    SDL_Window* sdlWindow;
    SDL_GLContext glContext;

    const int width;
    const int height;
    const double aspectRatio;
    const char* title;
    bool vsyncEnabled { false };
};

}  // namespace Rival
