#pragma once

#include "utils/SDLWrapper.h"

#include "application/Resources.h"
#include "application/Window.h"

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
     * Handles keyUp events.
     */
    virtual void keyUp(const SDL_Keycode) {};

    /**
     * Handles mouse down events.
     */
    virtual void mouseDown(const SDL_MouseButtonEvent) {}

    /**
     * Handles mouse up events.
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
     * to this method, equal to TimeUtils::timeStepMs.
     */
    virtual void update(int /*delta*/) = 0;

    /**
     * Renders the current frame.
     */
    virtual void render(int delta) = 0;

protected:
    Application& app;
    Window* window;
    Resources& res;
};

}  // namespace Rival
