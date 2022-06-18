#pragma once

#include <SDL.h>

#include "Resources.h"
#include "Window.h"

namespace Rival {

// Forward declaration to avoid circular reference
class Application;

class State
{

public:
    State(Application& app);

    virtual ~State() {}

    /**
     * Called when this State becomes active.
     */
    virtual void onLoad() {}

    /**
     * Handles keyDown events.
     */
    virtual void keyDown(const SDL_Keycode) {};

    /**
     * Handles mouse events.
     */
    virtual void mouseUp(const SDL_MouseButtonEvent) {}

    /**
     * Handles mouse wheel events.
     */
    virtual void mouseWheelMoved(const SDL_MouseWheelEvent) {}

    /**
     * Updates the logic.
     *
     * It is assumed that a fixed amount of time has elapsed between calls
     * to this method, equal to TimerUtils::timeStepMs.
     */
    virtual void update() = 0;

    /**
     * Renders the current frame.
     */
    virtual void render(int delta) = 0;

protected:
    Application& app;
    const Window& window;
    Resources& res;
};

}  // namespace Rival
