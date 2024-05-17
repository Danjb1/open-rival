#include "catch2/catch.h"

#include <memory>
#include <unordered_map>

#include "commands/GameCommand.h"
#include "entity/Unit.h"
#include "entity/components/MouseHandlerComponent.h"
#include "game/MousePicker.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"
#include "game/World.h"
#include "gfx/Camera.h"
#include "gfx/RenderUtils.h"
#include "mocks/MockSDL.h"
#include "utils/Rect.h"

using namespace Rival;

namespace TestMousePicker {

// TODO: Move this somewhere common
class DummyPlayerStore : public PlayerStore
{
private:
    std::unordered_map<int, PlayerState>& playerStates;

public:
    DummyPlayerStore(std::unordered_map<int, PlayerState>& playerStates)
        : playerStates(playerStates)
    {
    }

    int getNumPlayers() const override
    {
        return 1;
    }

    PlayerState& getLocalPlayerState() const override
    {
        auto result = playerStates.find(0);
        if (result == playerStates.end())
        {
            throw std::runtime_error("No local player state found!");
        }

        return result->second;
    }

    PlayerState* getPlayerState(int /*playerId*/) const override
    {
        return nullptr;
    }

    bool isLocalPlayer(int playerId) const override
    {
        return playerId == 0;
    }
};

// TODO: Move this somewhere common
class DummyGameCommandInvoker : public GameCommandInvoker
{
public:
    void dispatchCommand(std::shared_ptr<GameCommand> command) override {}
};

const int viewportWidth = 800;
const int viewportHeight = 600;
const float aspectRatio = static_cast<float>(viewportWidth) / viewportHeight;
Rect viewport(0, 0, viewportWidth, viewportHeight);
World world(50, 50, false);
PlayerContext playerContext;
std::unordered_map<int, PlayerState> playerStates;
DummyPlayerStore playerStore(playerStates);
DummyGameCommandInvoker cmdInvoker;

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
SCENARIO("Mouse picker should determine the tile under the mouse", "[mouse-picker]")
{
    // Create a pixel-perfect Camera
    float width = RenderUtils::pxToCamera_X(static_cast<float>(viewportWidth));
    Camera camera(0.0f, 0.0f, width, width / aspectRatio, world);

    GIVEN("the mouse is outside the viewport")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);

        REQUIRE(playerContext.tileUnderMouse.x == -1);
        REQUIRE(playerContext.tileUnderMouse.y == -1);

        MockSDL::mouseX = viewportWidth + 10;
        MockSDL::mouseY = viewportHeight + 10;

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the previous tile co-ordinates are returned")
            {
                REQUIRE(tileX == -1);
                REQUIRE(tileY == -1);
            }
        }
    }

    GIVEN("the mouse is in the centre of the screen")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = viewportWidth / 2;
        MockSDL::mouseY = viewportHeight / 2;

        AND_GIVEN("the camera is zoomed in at the centre of the map")
        {
            camera.centreOnTile(25, 25);
            camera.modZoom(0.5f);
            mousePicker.handleMouse();

            WHEN("getting the tile under the mouse")
            {
                int tileX = playerContext.tileUnderMouse.x;
                int tileY = playerContext.tileUnderMouse.y;

                THEN("the correct tile co-ordinates are returned")
                {
                    REQUIRE(tileX == 25);
                    REQUIRE(tileY == 25);
                }
            }
        }

        AND_GIVEN("the camera is zoomed out at the centre of the map")
        {
            camera.centreOnTile(25, 25);
            camera.modZoom(-0.5f);
            mousePicker.handleMouse();

            WHEN("getting the tile under the mouse")
            {
                int tileX = playerContext.tileUnderMouse.x;
                int tileY = playerContext.tileUnderMouse.y;

                THEN("the correct tile co-ordinates are returned")
                {
                    REQUIRE(tileX == 25);
                    REQUIRE(tileY == 25);
                }
            }
        }
    }

    GIVEN("the mouse is at the far corner of the map")
    {
        camera.centreOnTile(50, 50);
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = viewportWidth - 1;
        MockSDL::mouseY = viewportHeight - 1;
        mousePicker.handleMouse();

        WHEN("getting the tile under the mouse")
        {
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 49);
                REQUIRE(tileY == 49);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Even-column tile
    ///////////////////////////////////////////////////////////////////////////

    GIVEN("the mouse is in the top-left quadrant of an even-column tile")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        // Due to the way tiles overlap, the point at
        //  (tileWidthPx * 1.5f, tileHeightPx * 2.5f)
        // should be the centre of tile (2, 2).
        MockSDL::mouseX = static_cast<int>(RenderUtils::tileWidthPx * 1.45f);
        MockSDL::mouseY = static_cast<int>(RenderUtils::tileHeightPx * 2.45f);

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 2);
                REQUIRE(tileY == 2);
            }
        }
    }

    GIVEN("the mouse is in the top-right quadrant of an even-column tile")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = static_cast<int>(1.55f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.45f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 2);
                REQUIRE(tileY == 2);
            }
        }
    }

    GIVEN("the mouse is in the bottom-left quadrant of an even-column tile")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = static_cast<int>(1.45f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.55f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 2);
                REQUIRE(tileY == 2);
            }
        }
    }

    GIVEN("the mouse is in the bottom-right quadrant of an even-column tile")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = static_cast<int>(1.55f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.55f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 2);
                REQUIRE(tileY == 2);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Odd-column tile
    ///////////////////////////////////////////////////////////////////////////

    GIVEN("the mouse is in the top-left quadrant of an odd-column tile")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        // Due to the way tiles overlap, the point at
        //  (tileWidthPx * 1.0f, tileHeightPx * 2.0f)
        // should be the centre of tile (1, 1).
        MockSDL::mouseX = static_cast<int>(0.95f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(1.95f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 1);
                REQUIRE(tileY == 1);
            }
        }
    }

    GIVEN("the mouse is in the top-right quadrant of an odd-column tile")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = static_cast<int>(1.05f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(1.95f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 1);
                REQUIRE(tileY == 1);
            }
        }
    }

    GIVEN("the mouse is in the bottom-left quadrant of an odd-column tile")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = static_cast<int>(0.95f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.05f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 1);
                REQUIRE(tileY == 1);
            }
        }
    }

    GIVEN("the mouse is in the bottom-right quadrant of an odd-column tile")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = static_cast<int>(1.05f * RenderUtils::tileWidthPx);
        MockSDL::mouseY = static_cast<int>(2.05f * RenderUtils::tileHeightPx);

        WHEN("getting the tile under the mouse")
        {
            mousePicker.handleMouse();
            int tileX = playerContext.tileUnderMouse.x;
            int tileY = playerContext.tileUnderMouse.y;

            THEN("the correct tile co-ordinates are returned")
            {
                REQUIRE(tileX == 1);
                REQUIRE(tileY == 1);
            }
        }
    }
}

SCENARIO("Mouse picker should detect units under the mouse", "[mouse-picker]")
{
    // Add a Unit with a MouseHandlerComponent
    std::shared_ptr<Unit> unit = std::make_shared<Unit>(UnitType::Knight, "", false);
    unit->attach(std::make_shared<MouseHandlerComponent>());
    world.addEntity(unit, 4, 4);

    // Create a pixel-perfect Camera
    float width = RenderUtils::pxToCamera_X(static_cast<float>(viewportWidth));
    Camera camera(0.0f, 0.0f, width, width / aspectRatio, world);

    GIVEN("the mouse is not over a unit")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = 0;
        MockSDL::mouseY = 0;

        WHEN("getting the entity under the mouse")
        {
            mousePicker.handleMouse();
            std::shared_ptr<Entity> entity = playerContext.weakEntityUnderMouse.lock();

            THEN("no entity is returned")
            {
                REQUIRE(!entity);
            }
        }
    }
    GIVEN("the mouse is over a unit")
    {
        MousePicker mousePicker(camera, viewport, world, playerContext, playerStore, cmdInvoker);
        MockSDL::mouseX = static_cast<int>(RenderUtils::tileWidthPx * 2.5f);
        MockSDL::mouseY = static_cast<int>(RenderUtils::tileHeightPx * 4.5f);

        WHEN("getting the entity under the mouse")
        {
            mousePicker.handleMouse();
            std::shared_ptr<Entity> entity = playerContext.weakEntityUnderMouse.lock();

            THEN("the correct entity is returned")
            {
                REQUIRE(entity);
                REQUIRE(entity->getId() == 0);
            }
        }
    }
}

}  // namespace TestMousePicker
