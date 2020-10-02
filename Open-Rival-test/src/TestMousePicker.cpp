#include "pch.h"
#include "catch2/catch.h"

#include "Camera.h"
#include "MousePicker.h"
#include "RenderUtils.h"
#include "Scenario.h"
#include "SDL.h"

using namespace Rival;

const int viewportWidth = 800;
const int viewportHeight = 600;
const double aspectRatio =
        static_cast<double>(viewportWidth) / viewportHeight;
Rect viewport(0, 0, viewportWidth, viewportHeight);
Scenario scenario(50, 50, false);

/**
 * These tests can be a little difficult to understand.
 *
 * The easiest way to debug them may be to reproduce the environment from the
 * test in the real game, and print the mouse position each frame. That way,
 * you can literally see the rendered scene, so it is easy to figure out what
 * the mouse picker *should* be calculating.
 */
SCENARIO("mouse picker should determine the tile under the mouse", "[mouse-picker]") {

    // Create a pixel-perfect Camera
    Camera camera(0.0f, 0.0f,
            RenderUtils::pxToCamera_X(static_cast<float>(viewportWidth)),
            aspectRatio,
            scenario);

    GIVEN("the mouse is outside the viewport") {
        MousePicker mousePicker(camera, viewport, scenario);

        REQUIRE(mousePicker.getTileX() == 0);
        REQUIRE(mousePicker.getTileY() == 0);

        MockSDL::mouseX = viewportWidth + 10;
        MockSDL::mouseY = viewportHeight + 10;

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the previous tile co-ordinates are returned") {
                REQUIRE(tileX == 0);
                REQUIRE(tileY == 0);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Even-column tile
    ///////////////////////////////////////////////////////////////////////////

    GIVEN("the mouse is in the top-left quadrant of an even-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        // Due to the way tiles overlap, the point at
        //  (tileWidthPx * 1.5f, tileHeightPx * 2.5f)
        // should be the centre of tile (2, 2).
        MockSDL::mouseX = static_cast<int>(RenderUtils::tileWidthPx * 1.45f);
        MockSDL::mouseY = static_cast<int>(RenderUtils::tileHeightPx * 2.45f);

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 2);
                REQUIRE(tileY == 2);
            }
        }

        AND_GIVEN("the camera is zoomed in and panned to some location") {
            camera.translate(5, 5);
            camera.modZoom(0.1f);
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 7);
                REQUIRE(tileY == 7);
            }
        }
    }

    GIVEN("the mouse is in the top-right quadrant of an even-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = static_cast<int>(1.55f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.45f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 2);
                REQUIRE(tileY == 2);
            }
        }

        AND_GIVEN("the camera is zoomed in and panned to some location") {
            camera.translate(5, 5);
            camera.modZoom(0.1f);
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                // After translating by (5, 5), we might expect the resulting
                // tile to be (7, 7), but we have also zoomed towards the
                // camera centre, which brings the mouse to (8, 8).
                REQUIRE(tileX == 8);
                REQUIRE(tileY == 8);
            }
        }
    }

    GIVEN("the mouse is in the bottom-left quadrant of an even-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = static_cast<int>(1.45f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.55f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 2);
                REQUIRE(tileY == 2);
            }
        }

        AND_GIVEN("the camera is zoomed in and panned to some location") {
            camera.translate(5, 5);
            camera.modZoom(0.1f);
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 8);
                REQUIRE(tileY == 8);
            }
        }
    }

    GIVEN("the mouse is in the bottom-right quadrant of an even-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = static_cast<int>(1.55f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.55f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 2);
                REQUIRE(tileY == 2);
            }
        }

        AND_GIVEN("the camera is zoomed in and panned to some location") {
            camera.translate(5, 5);
            camera.modZoom(0.1f);
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 8);
                REQUIRE(tileY == 8);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Odd-column tile
    ///////////////////////////////////////////////////////////////////////////

    GIVEN("the mouse is in the top-left quadrant of an odd-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = 100;
        MockSDL::mouseY = 100;

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                // TODO
            }
        }
    }

    GIVEN("the mouse is in the top-right quadrant of an odd-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = 100;
        MockSDL::mouseY = 100;

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                // TODO
            }
        }
    }

    GIVEN("the mouse is in the bottom-left quadrant of an odd-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = 100;
        MockSDL::mouseY = 100;

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                // TODO
            }
        }
    }

    GIVEN("the mouse is in the bottom-right quadrant of an odd-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = 100;
        MockSDL::mouseY = 100;

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                // TODO
            }
        }
    }
}

SCENARIO("mouse picker should detect units under the mouse", "[mouse-picker]") {

    // Create a pixel-perfect Camera
    Camera camera(0.0f, 0.0f,
            RenderUtils::pxToCamera_X(static_cast<float>(viewportWidth)),
            aspectRatio,
            scenario);

    GIVEN("the mouse is over a unit") {
        MousePicker mousePicker(camera, viewport, scenario);
        // TODO

        WHEN("getting the entity under the mouse") {
            // TODO

            THEN("the correct entity ID is returned") {
                // TODO
            }
        }
    }
}
