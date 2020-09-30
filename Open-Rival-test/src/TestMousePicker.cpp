#include "pch.h"
#include "catch2/catch.h"

#include "Camera.h"
#include "MousePicker.h"
#include "Scenario.h"
#include "SDL.h"

SCENARIO("mouse picker should return the previous tile co-ordinates when the mouse is outside the viewport", "[mouse-picker]") {

    GIVEN("the mouse is outside the viewport") {
        const int viewportWidth = 800;
        const int viewportHeight = 600;
        const double aspectRatio =
                static_cast<double>(viewportWidth) / viewportHeight;
        Rival::Rect viewport(0, 0, viewportWidth, viewportHeight);
        Rival::Scenario scenario(50, 50, false);
        Rival::Camera camera(0.0f, 0.0f, 20.0f, aspectRatio, scenario);
        Rival::MousePicker mousePicker(camera, viewport, scenario);

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
}
