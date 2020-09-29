#include "pch.h"
#include "catch2/catch.h"

#include "MockResources.h"
#include "MockSDL.h"
#include "MockWindow.h"

#include "Application.h"

SCENARIO("logic is run multiple times if we are running behind", "[game-loop]") {

    GIVEN("An update was due more than 2 timestep intervals ago") {
        Rival::Window window(800, 600);
        Rival::Application app(window);

        WHEN("the game ticks") {
            // TODO

            THEN("the logic is run twice before rendering") {
                // TODO
                REQUIRE(true);
            }
        }
    }
}