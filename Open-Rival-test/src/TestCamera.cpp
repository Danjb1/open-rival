#include "pch.h"
#include "catch2/catch.h"

#include "Camera.h"

SCENARIO("camera should stay in bounds", "[camera]") {

    GIVEN("A camera looking at some point in a scenario") {
        Rival::Scenario scenario(50, 50, false);
        const double aspectRatio = static_cast<double>(4) / 3;
        Rival::Camera camera(25.0f, 25.0f, 20.0f, aspectRatio, scenario);

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

        WHEN("centering the camera on a tile in an even-numbered column") {
            camera.centreOnTile(16, 25);

            THEN("the camera points at the tile's centre") {
                // tileX * tileWidth + halfTileWidth
                //  16   *    2      +     1
                REQUIRE(camera.getX() == 33.0f);

                // tileY * tileHeight + halfTileHeight
                //  25   *     1      +     0.5
                REQUIRE(camera.getY() == 25.5f);
            }
        }

        WHEN("centering the camera on a tile in an odd-numbered column") {
            camera.centreOnTile(17, 25);

            THEN("the camera points at the tile's centre") {
                // tileX * tileWidth + halfTileWidth
                //  17   *    2      +     1
                REQUIRE(camera.getX() == 35.0f);

                // tileY * tileHeight + halfTileHeight + zigzagOffset
                //  25   *     1      +     0.5        +     0.5
                REQUIRE(camera.getY() == 26.0f);
            }
        }
    }
}
