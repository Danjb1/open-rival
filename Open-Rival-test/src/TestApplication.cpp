#include "pch.h"
#include "catch2/catch.h"

#include "Application.h"
#include "SDL.h"
#include "State.h"
#include "TimerUtils.h"

using namespace Rival;

SCENARIO("Logic is run multiple times if we are running behind", "[application][game-loop]") {

    /**
     * A State that takes a long time to render.
     *
     * Exits after 2 frames have passed (we need to simulate a second frame to give
     * the logic a chance to react to the slow rendering).
     */
    class SlowRenderingState : public State {
    public:
        int framesPassed = 0;
        int updatesRun = 0;

        SlowRenderingState(Application& app)
            : State(app) {}

        void update() override {
            updatesRun++;
        }

        void render() override {
            // Simulate 2 frame-lengths passing
            MockSDL::ticks += 2 * TimerUtils::timeStepMs;

            // Exit after the second frame
            framesPassed++;
            if (framesPassed == 2) {
                app.requestExit();
            }
        }
    };

    GIVEN("An Application that has not yet started") {
        Window window(800, 600, "Rival Realms");
        Application app(window);

        WHEN("the game ticks, and the first render takes too long") {
            std::unique_ptr<SlowRenderingState> initialState =
                    std::make_unique<SlowRenderingState>(app);
            app.start(std::move(initialState));

            THEN("the logic is run twice before the second render to catch up") {
                // The initialState that we created has been moved, so we need
                // to get a reference to it
                SlowRenderingState& state =
                        static_cast<SlowRenderingState&>(app.getState());
                REQUIRE(state.framesPassed == 2);
                REQUIRE(state.updatesRun == 3);
            }
        }
    }
}
