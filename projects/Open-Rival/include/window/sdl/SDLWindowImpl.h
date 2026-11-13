#pragma once

#include "utils/SDLWrapper.h"

#include <unordered_set>

#include "window/Window.h"

namespace Rival {

/** A window created with SDL.
 * When ENABLE_IMGUI is defined, this also hosts the ImGui platform backend, so an ImGui context must exist for the
 * lifetime of this window. */
class SDLWindowImpl : public Window
{
public:
    SDLWindowImpl(int width, int height, const char* title);
    ~SDLWindowImpl();

    void swapBuffers() const override;
    void setPos(int x, int y) override;
    void pollEvents() override;
#ifdef ENABLE_IMGUI
    void beginImGuiFrame() override;
#endif

private:
    static void initSDL();

    void handleEvent(const SDL_Event& e);

private:
    SDL_Window* sdlWindow = nullptr;
    SDL_GLContext glContext = nullptr;

    /** Keys whose key-down event was passed to the listener (i.e. not captured by ImGui).
     * We only pass on key-up events for these keys, so the listener always sees matching pairs. */
    std::unordered_set<SDL_Keycode> keysForwardedToListener;
};

}  // namespace Rival
