#ifndef STATE_H
#define STATE_H

#include <SDL.h>

namespace Rival {

    // Forward declaration to avoid circular reference
    class Application;

    class State {

    public:
        virtual void initialize(Application* app) = 0;

        /**
         * Handles keyDown events.
         */
        virtual void keyDown(const SDL_Keycode keyCode) = 0;

        /**
         * Handles mouse wheel events.
         */
        virtual void mouseWheelMoved(const SDL_MouseWheelEvent evt) = 0;

        /**
         * Renders the current frame.
         */
        virtual void render() = 0;

        /**
         * Updates the game.
         */
        virtual void update() = 0;
    };

}  // namespace Rival

#endif  // STATE_H
