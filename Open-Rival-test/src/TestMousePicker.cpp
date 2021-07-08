#include "pch.h"
#include "catch2/catch.h"

#include <memory>

#include "Camera.h"
#include "MousePicker.h"
#include "RenderUtils.h"
#include "Scenario.h"
#include "SDL.h"

using namespace Rival;

const int viewportWidth = 800;
const int viewportHeight = 600;
const float aspectRatio =
        static_cast<float>(viewportWidth) / viewportHeight;
Rect viewport(0, 0, viewportWidth, viewportHeight);
Scenario scenario(50, 50, false);

/**
 * These tests can be a little difficult to understand.
 *
 * The easiest way to debug them may be to reproduce the environment from the
 * test in the real game, and print the mouse position each frame:
 *
 *     std::cout << mouseX << ", " << mouseY << " -> " << tile.first << ", " << tile.second << "\n";
 *
 * That way, you can literally see the rendered scene, so it is easy to figure
 * out what the mouse picker *should* be calculating.
 *
 * Alternatively, mouse positions can be plotted using `tile_grid.psd`.
 */
SCENARIO("Mouse picker should determine the tile under the mouse", "[mouse-picker]") {

    // Create a pixel-perfect Camera
    float width = RenderUtils::pxToCamera_X(static_cast<float>(viewportWidth));
    Camera camera(0.0f, 0.0f,
            width,
            width / aspectRatio,
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

    GIVEN("the mouse is in the centre of the screen") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = viewportWidth / 2;
        MockSDL::mouseY = viewportHeight / 2;

        AND_GIVEN("the camera is zoomed in at the centre of the map") {
            camera.centreOnTile(25, 25);
            camera.modZoom(0.5f);
            mousePicker.handleMouse();

            WHEN("getting the tile under the mouse") {
                int tileX = mousePicker.getTileX();
                int tileY = mousePicker.getTileY();

                THEN("the correct tile co-ordinates are returned") {
                    REQUIRE(tileX == 25);
                    REQUIRE(tileY == 25);
                }
            }
        }

        AND_GIVEN("the camera is zoomed out at the centre of the map") {
            camera.centreOnTile(25, 25);
            camera.modZoom(-0.5f);
            mousePicker.handleMouse();

            WHEN("getting the tile under the mouse") {
                int tileX = mousePicker.getTileX();
                int tileY = mousePicker.getTileY();

                THEN("the correct tile co-ordinates are returned") {
                    REQUIRE(tileX == 25);
                    REQUIRE(tileY == 25);
                }
            }
        }
    }

    GIVEN("the mouse is at the far corner of the map") {
        camera.centreOnTile(50, 50);
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = viewportWidth - 1;
        MockSDL::mouseY = viewportHeight - 1;
        mousePicker.handleMouse();

        WHEN("getting the tile under the mouse") {
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 49);
                REQUIRE(tileY == 49);
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
    }

    ///////////////////////////////////////////////////////////////////////////
    // Odd-column tile
    ///////////////////////////////////////////////////////////////////////////

    GIVEN("the mouse is in the top-left quadrant of an odd-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        // Due to the way tiles overlap, the point at
        //  (tileWidthPx * 1.0f, tileHeightPx * 2.0f)
        // should be the centre of tile (1, 1).
        MockSDL::mouseX = static_cast<int>(0.95f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(1.95f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 1);
                REQUIRE(tileY == 1);
            }
        }
    }

    GIVEN("the mouse is in the top-right quadrant of an odd-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = static_cast<int>(1.05f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(1.95f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 1);
                REQUIRE(tileY == 1);
            }
        }
    }

    GIVEN("the mouse is in the bottom-left quadrant of an odd-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = static_cast<int>(0.95f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.05f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 1);
                REQUIRE(tileY == 1);
            }
        }
    }

    GIVEN("the mouse is in the bottom-right quadrant of an odd-column tile") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = static_cast<int>(1.05f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.05f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse") {
            mousePicker.handleMouse();
            int tileX = mousePicker.getTileX();
            int tileY = mousePicker.getTileY();

            THEN("the correct tile co-ordinates are returned") {
                REQUIRE(tileX == 1);
                REQUIRE(tileY == 1);
            }
        }
    }
}

SCENARIO("Mouse picker should detect units under the mouse", "[mouse-picker]") {

    // Add a Unit
    std::unique_ptr<Entity> unit = std::make_unique<Entity>(1, 1);
    scenario.addEntity(std::move(unit), 4, 4);

    // Create a pixel-perfect Camera
    float width = RenderUtils::pxToCamera_X(static_cast<float>(viewportWidth));
    Camera camera(0.0f, 0.0f,
            width,
            width / aspectRatio,
            scenario);

    GIVEN("the mouse is not over a unit") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = 0;
        MockSDL::mouseY = 0;

        WHEN("getting the entity under the mouse") {
            mousePicker.handleMouse();
            int entityId = mousePicker.getEntityId();

            THEN("the value -1 is returned") {
                REQUIRE(entityId == -1);
            }
        }
    }
    GIVEN("the mouse is over a unit") {
        MousePicker mousePicker(camera, viewport, scenario);
        MockSDL::mouseX = static_cast<int>(RenderUtils::tileWidthPx * 2.5f);
        MockSDL::mouseY = static_cast<int>(RenderUtils::tileHeightPx * 4.5f);

        WHEN("getting the entity under the mouse") {
            mousePicker.handleMouse();
            int entityId = mousePicker.getEntityId();

            THEN("the correct entity ID is returned") {
                REQUIRE(entityId == 0);
            }
        }
    }
}
