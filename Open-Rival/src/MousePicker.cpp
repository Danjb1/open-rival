#include "pch.h"
#include "MousePicker.h"

#include <SDL.h>
#include <iostream>

#include "Entity.h"
#include "MathUtils.h"
#include "Unit.h"

namespace Rival {

    MousePicker::MousePicker(
            Camera& camera,
            Rect& viewport,
            Scenario& scenario)
        : camera(camera),
          viewport(viewport),
          scenario(scenario),
          mapWidth(scenario.getWidth()),
          mapHeight(scenario.getHeight()),
          entityId(-1) {}

    void MousePicker::handleMouse() {

        // Reset selected entity
        entityId = -1;

        // Get the mouse position relative to the window, in pixels
        int mouseX;
        int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Abort if the mouse is outside the viewport
        if (!viewport.contains(mouseX, mouseY)) {
            return;
        }

        // Calculate the mouse position relative to the viewport, in pixels
        int mouseInViewportX = mouseX - static_cast<int>(viewport.x);
        int mouseInViewportY = mouseY - static_cast<int>(viewport.y);

        // Calculate the mouse position in camera units
        float mouseCameraX = getMouseInCameraX(mouseInViewportX);
        float mouseCameraY = getMouseInCameraY(mouseInViewportY);

        // Figure out what's under the mouse
        tile = getTilePos(mouseCameraX, mouseCameraY);
        findEntityUnderMouse(mouseInViewportX, mouseInViewportY);
    }

    float MousePicker::getMouseInCameraX(int mouseInViewportX) {
        // Calculate mouse position relative to the viewport, in the range 0-1
        float normalizedMouseX =
                static_cast<float>(mouseInViewportX) / viewport.width;
        return camera.getLeft() + normalizedMouseX * camera.getWidth();
    }

    float MousePicker::getMouseInCameraY(int mouseInViewportY) {
        // Calculate mouse position relative to the viewport, in the range 0-1
        float normalizedMouseY =
                static_cast<float>(mouseInViewportY) / viewport.height;
        return camera.getTop() + normalizedMouseY * camera.getHeight();
    }

    std::pair<int, int> MousePicker::getTilePos(
            float mouseCameraX,
            float mouseCameraY) {

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
         *      |------------+------------B------------+------------|
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
        if (tileX % 2 == 0) {

            // Even-column tiles are positioned "normally" (no extra y-offset).
            // This results in an offsetY of:
            //  - 0 at the top of the tile
            //  - 1 at the bottom of the tile

            if (offsetY < 0.5f) {
                // Top-left quadrant
                //  => Diagonal: 2y + x = 1
                if (2 * offsetY + offsetX < 1) {
                    tileX--;
                    tileY--;
                }
            } else {
                // Bottom-left quadrant
                //  => Diagonal: 2y - x = 1
                if (2 * offsetY - offsetX > 1) {
                    tileX--;
                }
            }

        } else {

            // Odd-column tiles are positioned with an extra y-offset.
            // This results in an offsetY of:
            //  - 0 at the CENTRE of a tile
            //  - 0.5 at the bottom of that tile
            //  - 1 at the centre of the next tile down

            if (offsetY < 0.5f) {
                // Bottom-left quadrant
                //  => Diagonal: 2y - x = 0
                if (2 * offsetY - offsetX > 0) {
                    tileX--;
                } else {
                    tileY--;
                }
            } else {
                // Top-left quadrant
                //  => Diagonal: 2y + x = 2
                if (2 * offsetY + offsetX < 2) {
                    tileX--;
                }
            }
        }

        // By this point tileX and tileY are correct,
        // but we should limit them to the level bounds
        tileX = MathUtils::clampi(tileX, 0, mapWidth - 1);
        tileY = MathUtils::clampi(tileY, 0, mapHeight - 1);

        return std::pair<int, int>(tileX, tileY);
    }

    void MousePicker::findEntityUnderMouse(
            int mouseInViewportX,
            int mouseInViewportY) {
        auto& entities = scenario.getEntities();
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            const Entity& entity = *it->second;
            if (isMouseInEntity(entity, mouseInViewportX, mouseInViewportY)) {
                entityId = entity.getId();
                std::cout << "Entity " << entityId << " of type unit ("
                          << static_cast<int>(entity.getType())
                          << ") is under cursor.\n";
                break;
            }
        }
    }

    bool MousePicker::isMouseInEntity(
            const Entity& entity,
            int mouseInViewportX,
            int mouseInViewportY) {

        /*
         * Units are always rendered at a fixed pixel offset from the tile they
         * occupy. Thus, if we can figure out the rendered position of this
         * tile, we can figure out the rendered position of the unit.
         */

        // Find the rendered position of the top-left corner of the unit's
        // tile. This is measured in scaled px, which takes into account the
        // zoom level used during rendering.
        //
        // IMPORTANT: This means that any subsequent operations affecting this
        // position must use the same units, that is, we have to always take
        // the zoom level into account!
        float zoom = camera.getZoom();
        float tileX_px = RenderUtils::tileToScaledPx_X(entity.getX(), zoom);
        float tileY_px = RenderUtils::tileToScaledPx_Y(
                entity.getX(), entity.getY(), zoom);

        // Adjust based on the camera position
        // (as the camera pans right, tiles are rendered further to the left!)
        tileX_px -= RenderUtils::cameraToPx_X(camera.getLeft()) * zoom;
        tileY_px -= RenderUtils::cameraToPx_Y(camera.getTop()) * zoom;

        // Find the bottom-left corner of the unit hitbox
        float unitX1 = tileX_px + (unitHitboxOffsetX * zoom);
        float unitY2 = tileY_px + (unitHitboxOffsetY * zoom);

        // Now find the opposite corner based on the hitbox size
        // TMP: For now we use a fixed height for all units
        float unitX2 = unitX1 + (unitHitboxWidth * zoom);
        float unitY1 = unitY2 - (unitHitboxHeight * zoom);

        // Finally, see if the mouse is inside the hitbox
        return mouseInViewportX >= unitX1
                && mouseInViewportY >= unitY1
                && mouseInViewportX < unitX2
                && mouseInViewportY < unitY2;
    }

    int MousePicker::getTileX() const {
        return tile.first;
    }

    int MousePicker::getTileY() const {
        return tile.second;
    }

    int MousePicker::getEntityId() const {
        return entityId;
    }

}  // namespace Rival
