#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "AudioSystem.h"
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

        void setState(std::unique_ptr<State> newState);

        const Window& getWindow() const;
        AudioSystem& getAudioSystem();
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

        bool exiting { false };

        Window& window;
        AudioSystem audioSystem;
        Resources res;

        std::unique_ptr<State> state;
        std::unique_ptr<State> nextState;

        void makeNextStateActive();
    };

}  // namespace Rival

#endif  // APPLICATION_H
