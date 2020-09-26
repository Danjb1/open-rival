#include "pch.h"
#include "Application.h"

namespace Rival {

    bool vsyncEnabled = true;

    Application::Application(Window& window)
        : window(window) {
    }

    void Application::start(State& initialState) {

        // Event handler
        SDL_Event e;

        // Initialise the State
        State& state = initialState;
        state.initialize(this);

        bool exiting = false;
        Uint32 nextUpdateDue = SDL_GetTicks();

        // Game loop
        while (!exiting) {
            Uint32 frameStartTime = SDL_GetTicks();

            // Is the next update due?
            if (vsyncEnabled || nextUpdateDue <= frameStartTime) {

                // Handle events on the queue
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        exiting = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        state.keyDown(e.key.keysym.sym);
                    } else if (e.type == SDL_MOUSEWHEEL) {
                        state.mouseWheelMoved(e.wheel);
                    }
                }

                // Update the game logic, as many times as necessary to keep it
                // in-sync with the refresh rate.
                //
                // For example:
                //  - For a 30Hz monitor, this will run twice per render.
                //  - For a 60Hz monitor, this will run once per render.
                //  - For a 120Hz monitor, this will run every other render.
                //
                // If vsync is disabled, this should run once per render.
                while (nextUpdateDue <= frameStartTime) {
                    state.update();
                    nextUpdateDue += TimerUtils::timeStepMs;
                }

                // Render the game, once per iteration.
                // With vsync enabled, this matches the screen's refresh rate.
                // Otherwise, this matches our target FPS.
                state.render();

                // Update the window with our newly-rendered game.
                // If vsync is enabled, this will block execution until the
                // next swap interval.
                window.swapBuffers();

            } else {
                // Next update is not yet due.
                // Sleep for the shortest possible time, so as not to risk
                // overshooting!
                SDL_Delay(1);
            }
        }

        // Free resources and exit SDL
        exit();
    }

    void Application::exit() {
        SDL_Quit();
    }

    Window& Application::getWindow() {
        return window;
    }

    Resources& Application::getResources() {
        return res;
    }

}  // namespace Rival
