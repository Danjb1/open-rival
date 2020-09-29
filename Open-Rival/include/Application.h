#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

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
        void start(std::unique_ptr<State> state);

        void pollEvents();

        void requestExit();

        const Window& getWindow() const;
        Resources& getResources();
        State& getState();

    private:
        /**
         * Minimum time that we will consider sleeping for.
         *
         * If the next frame is due sooner than this then we will just
         * busy-wait, to reduce the risk of oversleeping.
         */
        static const int minSleepTime = 2;

        bool exiting;
        Window& window;
        Resources res;
        std::unique_ptr<State> state;
    };

}  // namespace Rival

#endif  // APPLICATION_H
