#include "catch2/catch.h"

#include "game/World.h"
#include "gfx/Camera.h"

using namespace Rival;

namespace TestCamera {

float aspectRatio = (16.0f / 9);

SCENARIO("Camera should stay in bounds", "[camera]")
{
    GIVEN("A camera looking at some point in a world")
    {
        World world(50, 50, false);
        Camera camera(25.0f, 25.0f, 20.0f, 20.0f / aspectRatio, world);

        WHEN("moving the camera left")
        {
            camera.translate(-100.0f, 0.0f);

            THEN("the camera stays in bounds")
            {
                REQUIRE(camera.getLeft() == 0.0f);
            }
        }

        WHEN("moving the camera right")
        {
            camera.translate(100.0f, 0.0f);

            THEN("the camera stays in bounds")
            {
                REQUIRE(camera.getRight() == 51.0f);
            }
        }

        WHEN("moving the camera up")
        {
            camera.translate(0.0f, -100.0f);

            THEN("the camera stays in bounds")
            {
                REQUIRE(camera.getTop() == 0.0f);
            }
        }

        WHEN("moving the camera down")
        {
            camera.translate(0.0f, 100.0f);

            THEN("the camera stays in bounds")
            {
                REQUIRE(camera.getBottom() == 50.5f);
            }
        }
    }
}

SCENARIO("Camera should point at the centre of a tile", "[camera]")
{
    GIVEN("A camera looking at some point in a world")
    {
        World world(50, 50, false);
        Camera camera(25.0f, 25.0f, 20.0f, 20.0f / aspectRatio, world);

        WHEN("centering the camera on a tile in an even-numbered column")
        {
            camera.centreOnTile(16, 25);

            THEN("the camera points at the tile's centre")
            {
                // tileX + halfTileWidth
                //  16   +     1
                REQUIRE(camera.getPosition().x == 17.0f);

                // tileY + halfTileHeight
                //  25   +     0.5
                REQUIRE(camera.getPosition().y == 25.5f);
            }
        }

        WHEN("centering the camera on a tile in an odd-numbered column")
        {
            camera.centreOnTile(17, 25);

            THEN("the camera points at the tile's centre")
            {
                // tileX + halfTileWidth
                //  17   +     1
                REQUIRE(camera.getPosition().x == 18.0f);

                // tileY + halfTileHeight + zigzagOffset
                //  25   +     0.5        +     0.5
                REQUIRE(camera.getPosition().y == 26.0f);
            }
        }
    }
}

SCENARIO("Camera should be the correct size", "[camera]")
{
    GIVEN("A camera looking at some point in a world")
    {
        World world(50, 50, false);
        Camera camera(25.0f, 25.0f, 20.0f, 20.0f / aspectRatio, world);

        WHEN("getting the camera size")
        {
            float cameraWidth = camera.getWidth();
            float cameraHeight = camera.getHeight();

            THEN("the correct dimensions are returned")
            {
                REQUIRE(cameraWidth == 20.0f);
                REQUIRE(cameraHeight == 11.25f);
            }
        }

        AND_GIVEN("The camera is zoomed out")
        {
            camera.modZoom(-0.5f);

            WHEN("getting the camera size")
            {
                float cameraWidth = camera.getWidth();
                float cameraHeight = camera.getHeight();

                THEN("the correct dimensions are returned")
                {
                    // At a zoom level of 0.5,
                    // twice as many tiles should be visible
                    REQUIRE(cameraWidth == 40.0f);
                    REQUIRE(cameraHeight == 22.5f);
                }
            }
        }

        AND_GIVEN("The camera is zoomed in")
        {
            camera.modZoom(0.5f);

            WHEN("getting the camera size")
            {
                float cameraWidth = camera.getWidth();
                float cameraHeight = camera.getHeight();

                THEN("the correct dimensions are returned")
                {
                    // At a zoom level of 1.5 (3/2),
                    // 2/3 of the tiles should be visible
                    REQUIRE(cameraWidth == Approx(13.3333f));
                    REQUIRE(cameraHeight == Approx(7.5f));
                }
            }
        }
    }
}

}  // namespace TestCamera
