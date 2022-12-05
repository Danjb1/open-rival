#include "pch.h"

#include "MousePicker.h"

#include "SDLWrapper.h"

#include <cstdint>
#include <iostream>

#include "Camera.h"
#include "Entity.h"
#include "MapUtils.h"
#include "MathUtils.h"
#include "MouseHandlerComponent.h"
#include "MouseUtils.h"
#include "Rect.h"
#include "World.h"

namespace Rival {

MousePicker::MousePicker(Camera& camera, Rect& viewport, World& world, PlayerStore& playerStore)
    : camera(camera)
    , viewport(viewport)
    , world(world)
    , playerStore(playerStore)
    , mapWidth(world.getWidth())
    , mapHeight(world.getHeight())
    , tileUnderMouse({ -1, -1 })
{
}

void MousePicker::mouseDown()
{
    // TODO: Initiate drag-select
}

void MousePicker::mouseUp(std::uint8_t button)
{
    if (button == SDL_BUTTON_LEFT)
    {
        const auto entityUnderMouse = weakEntityUnderMouse.lock();
        if (entityUnderMouse)
        {
            entitySelected(entityUnderMouse);
        }
        else
        {
            tileSelected();
        }
    }
    else if (button == SDL_BUTTON_RIGHT)
    {
        deselect();
    }
}

void MousePicker::handleMouse()
{
    // Get the mouse position relative to the window, in pixels
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Abort if the mouse is outside the viewport
    if (!viewport.contains(mouseX, mouseY))
    {
        return;
    }

    // Calculate the mouse position relative to the viewport, in pixels
    int mouseInViewportX = MouseUtils::getMouseInViewportX(mouseX, viewport);
    int mouseInViewportY = MouseUtils::getMouseInViewportY(mouseY, viewport);

    // Calculate the mouse position relative to the viewport,
    // in the range 0-1.
    float normalizedMouseX = MouseUtils::getNormalizedMouseInViewportX(mouseX, viewport);
    float normalizedMouseY = MouseUtils::getNormalizedMouseInViewportY(mouseY, viewport);

    // Calculate the mouse position in camera units
    float mouseCameraX = getMouseInCameraX(normalizedMouseX);
    float mouseCameraY = getMouseInCameraY(normalizedMouseY);

    // Figure out what's under the mouse
    tileUnderMouse = getTilePos(mouseCameraX, mouseCameraY);
    weakEntityUnderMouse = findEntityUnderMouse(mouseInViewportX, mouseInViewportY);
}

float MousePicker::getMouseInCameraX(float normalizedMouseX)
{
    return camera.getLeft() + normalizedMouseX * camera.getWidth();
}

float MousePicker::getMouseInCameraY(float normalizedMouseY)
{
    return camera.getTop() + normalizedMouseY * camera.getHeight();
}

MapNode MousePicker::getTilePos(float mouseCameraX, float mouseCameraY)
{

    /*
     * The goal here is to reverse-engineer the rendering process to figure
     * out which tile contains the given point.
     *
     * Here is a visual representation of what we're dealing with:
     *
     *      0            1            2            3            4
     *      +------------+------------+------------+------------+
     *      |         #  |  #         |         #  |  #         |
     *      |      #     |     #      |      #     |     #      |
     *      |   #        |        #   |   #        |        #   |
     *      |<-----------A----------->|<-----------C----------->|
     *      |   #        |        #   |   #        |        #   |
     *      |      #     |     #      |      #     |     #      |
     *      |         #  |  #         |         #  |  #         |
     *      +------------+------------B------------+------------+
     *      |         #  |  #         |         #  |  #         |
     *      |      #     |     #      |      #     |     #      |
     *      |   #        |        #   |   #        |        #   |
     *      |<-----------+----------->|<-----------+----------->|
     *
     * Here, A and C represent even-column tiles, and B represents an
     * odd-column tile. Notice how B is offset in the y-axis; this is
     * something we will need to take into account.
     *
     * Notice also how each tile is framed in a rectangle, each rectangle
     * is divided into 4 quadrants, and each of these quadrants includes
     * a tile boundary. Therefore, even if we know which quadrant the point
     * is in, we still need to figure out which side of the tile boundary
     * it lies on.
     */

    // Get the naive tile position.
    // If our tiles were arranged in a regular axis-aligned grid then this
    // would be sufficient, but since they are diamond-shaped and
    // positioned in a zigzag pattern, we will need to make further
    // adjustments. However, this is a good starting point as it basically
    // tells us which quadrant we are in.
    int tileX = static_cast<int>(mouseCameraX);
    int tileY = static_cast<int>(mouseCameraY);

    // Get the "remainder" (just the decimal part of the world position).
    // Because of the way the tiles overlap, offsetX now ranges from 0-1,
    // where 0 is the left of a tile and 1 is the CENTRE of a tile; going
    // past the centre of a tile actually puts us in the next tile over
    // (tileX + 1). This means we are always considering only the LEFT side
    // of a tile. The meaning of offsetY is more complicated due to the
    // zigzagging tile positioning (keep reading).
    float offsetX = mouseCameraX - tileX;
    float offsetY = mouseCameraY - tileY;

    // Now that we know our naive tile co-ordinates and the mouse offset
    // within that tile, we can figure out the real tile. Based on the
    // quadrant, and which side of the tile boundary we are on, we can
    // figure out what adjustments we need to make to our naive tile
    // co-ordinates.
    if (MapUtils::isUpperTile(tileX))
    {

        // Even-column tiles are positioned "normally" (no extra y-offset).
        // This results in an offsetY of:
        //  - 0 at the top of the tile
        //  - 1 at the bottom of the tile

        if (offsetY < 0.5f)
        {
            // Top-left quadrant
            //  => Diagonal: 2y + x = 1
            if (2 * offsetY + offsetX < 1)
            {
                tileX--;
                tileY--;
            }
        }
        else
        {
            // Bottom-left quadrant
            //  => Diagonal: 2y - x = 1
            if (2 * offsetY - offsetX > 1)
            {
                tileX--;
            }
        }
    }
    else
    {

        // Odd-column tiles are positioned with an extra y-offset.
        // This results in an offsetY of:
        //  - 0 at the CENTRE of a tile
        //  - 0.5 at the bottom of that tile
        //  - 1 at the centre of the next tile down

        if (offsetY < 0.5f)
        {
            // Bottom-left quadrant
            //  => Diagonal: 2y - x = 0
            if (2 * offsetY - offsetX > 0)
            {
                tileX--;
            }
            else
            {
                tileY--;
            }
        }
        else
        {
            // Top-left quadrant
            //  => Diagonal: 2y + x = 2
            if (2 * offsetY + offsetX < 2)
            {
                tileX--;
            }
        }
    }

    // By this point tileX and tileY are correct,
    // but we should limit them to the level bounds
    tileX = MathUtils::clampi(tileX, 0, mapWidth - 1);
    tileY = MathUtils::clampi(tileY, 0, mapHeight - 1);

    return { tileX, tileY };
}

std::weak_ptr<Entity> MousePicker::findEntityUnderMouse(int mouseInViewportX, int mouseInViewportY)
{
    // Find the camera position, in pixels
    float cameraX_px = RenderUtils::cameraToPx_X(camera.getLeft());
    float cameraY_px = RenderUtils::cameraToPx_Y(camera.getTop());

    // Find the mouse position in the world by reversing the camera transform
    float zoom = camera.getZoom();
    float mouseInWorldX = (mouseInViewportX / zoom) + cameraX_px;
    float mouseInWorldY = (mouseInViewportY / zoom) + cameraY_px;

    // TODO: We could optimise this by considering only Entities that were rendered in the previous frame.
    const auto& entities = world.getMutableEntities();
    for (const auto& e : entities)
    {
        MouseHandlerComponent* mouseHandlerComponent =
                e->getComponent<MouseHandlerComponent>(MouseHandlerComponent::key);
        if (!mouseHandlerComponent)
        {
            continue;
        }

        const Rect hitbox = mouseHandlerComponent->getHitbox();
        if (hitbox.contains(mouseInWorldX, mouseInWorldY))
        {
            return e;
        }
    }

    return {};
}

MapNode MousePicker::getTilePos() const
{
    return tileUnderMouse;
}

void MousePicker::entitySelected(std::shared_ptr<Entity> entity)
{
    if (const auto mouseHandlerComponent = entity->getComponent<MouseHandlerComponent>(MouseHandlerComponent::key))
    {
        currentSelection.weakSelectedEntity = entity;
        mouseHandlerComponent->onSelect(playerStore);
    }
}

void MousePicker::tileSelected()
{
    if (const auto selectedEntity = currentSelection.weakSelectedEntity.lock())
    {
        if (const auto mouseHandlerComponent =
                    selectedEntity->getComponent<MouseHandlerComponent>(MouseHandlerComponent::key))
        {
            mouseHandlerComponent->onTileClicked(playerStore, tileUnderMouse);
        }
    }
}

void MousePicker::deselect()
{
    currentSelection.weakSelectedEntity = {};
}

}  // namespace Rival
