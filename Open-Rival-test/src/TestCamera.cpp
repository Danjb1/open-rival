#include "pch.h"
#include "catch2/catch.h"

#include "Camera.h"

SCENARIO("camera should stay in bounds", "[camera]") {

    GIVEN("A camera looking at some point in a scenario") {
        Rival::Scenario scenario(50, 50, false);
        Rival::Camera camera(25.0f, 25.0f, 20.0f, static_cast<double>(16) / 9,
                scenario);

        WHEN("moving the camera left") {
            camera.translate(-100.0f, 0.0f);

            THEN("the camera stays in bounds") {
                REQUIRE(camera.getLeft() == 0.0f);
            }
        }

        WHEN("moving the camera right") {
            camera.translate(100.0f, 0.0f);

            THEN("the camera stays in bounds") {
                REQUIRE(camera.getRight() == 51.0f);
            }
        }

        WHEN("moving the camera up") {
            camera.translate(0.0f, -100.0f);

            THEN("the camera stays in bounds") {
                REQUIRE(camera.getTop() == 0.0f);
            }
        }

        WHEN("moving the camera down") {
            camera.translate(0.0f, 100.0f);

            THEN("the camera stays in bounds") {
                REQUIRE(camera.getBottom() == 50.5f);
            }
        }
    }
}

SCENARIO("camera should point at the centre of a tile", "[camera]") {

    GIVEN("A camera looking at some point in a scenario") {
        Rival::Scenario scenario(50, 50, false);
        Rival::Camera camera(25.0f, 25.0f, 20.0f, static_cast<double>(16) / 9,
                scenario);

        WHEN("centering the camera on a tile") {
            camera.centreOnTile(15, 15);

            THEN("the camera points at the tile's centre") {
                REQUIRE(camera.getX() == 16.0f);
                REQUIRE(camera.getY() == 15.5f);
            }
        }
    }
}
