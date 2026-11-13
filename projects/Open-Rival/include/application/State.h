#pragma once

#include "application/Resources.h"
#include "input/Input.h"
#include "utils/TimeUtils.h"
#include "window/Window.h"

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
    virtual void keyDown(const KeyEvent&) {}

    /**
     * Handles keyUp events.
     */
    virtual void keyUp(const KeyEvent&) {}

    /**
     * Handles mouse down events.
     */
    virtual void mouseDown(const MouseButtonEvent&) {}

    /**
     * Handles mouse up events.
     */
    virtual void mouseUp(const MouseButtonEvent&) {}

    /**
     * Handles mouse wheel events.
     */
    virtual void mouseWheelMoved(const MouseWheelEvent&) {}

    /**
     * Updates the logic based on the given delta (milliseconds).
     * Using an integer delta value keeps things deterministic across different systems.
     */
    virtual void update(int delta) = 0;

    /**
     * Renders the current frame.
     * Here we can use more precise (floating-point) timings, since the rendering doesn't affect the game logic.
     */
    virtual void render(const FrameTiming& frameTiming) = 0;

    const Resources& getResources() const;

protected:
    Application& app;
    Window* window;
    Resources& res;
};

}  // namespace Rival
