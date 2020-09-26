#ifndef STATE_H
#define STATE_H

#include <SDL.h>

namespace Rival {

    // Forward declaration to avoid circular reference
    class Application;

    class State {

    public:
        /**
         * Handles keyDown events.
         */
        virtual void keyDown(const SDL_Keycode keyCode) = 0;

        /**
         * Handles mouse wheel events.
         */
        virtual void mouseWheelMoved(const SDL_MouseWheelEvent evt) = 0;

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
        virtual void render() = 0;
    };

}  // namespace Rival

#endif  // STATE_H
