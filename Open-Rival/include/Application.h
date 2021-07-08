#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "AudioSystem.h"
#include "json.h"
#include "Resources.h"
#include "State.h"
#include "Window.h"

using json = nlohmann::json;

namespace Rival {

    class Application {

    public:
        bool vsyncEnabled;

        Application(Window& window, json cfg);

        /**
         * Runs the Application until the user exits.
         */
        void start(std::unique_ptr<State> state);

        void pollEvents();

        void requestExit() { exiting = true; }

        void setState(std::unique_ptr<State> newState) {
            nextState = std::move(newState);
        }

        const Window& getWindow() const { return window; }
        AudioSystem& getAudioSystem() { return audioSystem; }
        Resources& getResources() { return res; }
        State& getState() { return *state; }

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
        json& cfg;
        AudioSystem audioSystem;
        Resources res;

        std::unique_ptr<State> state;
        std::unique_ptr<State> nextState;

        void makeNextStateActive();
    };

}  // namespace Rival

#endif  // APPLICATION_H
