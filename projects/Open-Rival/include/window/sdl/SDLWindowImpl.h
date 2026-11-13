#pragma once

#include "utils/SDLWrapper.h"

#include "window/Window.h"

namespace Rival {

/** A window created with SDL. */
class SDLWindowImpl : public Window
{
public:
    SDLWindowImpl(int width, int height, const char* title);
    ~SDLWindowImpl();

    void swapBuffers() const override;
    void setPos(int x, int y) override;

    void setIcon(SDL_Surface* surface);

private:
    static void initSDL();

private:
    SDL_Window* sdlWindow = nullptr;
    SDL_GLContext glContext = nullptr;
};

}  // namespace Rival
