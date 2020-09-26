#ifndef APPLICATION_H
#define APPLICATION_H

#include "Resources.h"
#include "State.h"
#include "Window.h"

namespace Rival {

    class Application {

    public:
        bool vsyncEnabled;

        Application(Window& window);

        /**
         * Runs the Application until the user exits.
         */
        void start(State& state);

        /**
         * Exits the Application cleanly.
         */
        void exit();

        Window& getWindow();
        Resources& getResources();

    private:
        Window& window;
        Resources res;

        void initGLEW();
        void initGL();
    };

}  // namespace Rival

#endif  // APPLICATION_H
