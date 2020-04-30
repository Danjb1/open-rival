#include "pch.h"
#include "MousePicker.h"

#include <SDL.h>

#include "MathUtils.h"
#include "Rival.h"

namespace Rival {

    MousePicker::MousePicker(
            Camera& camera,
            std::shared_ptr<Scenario> scenario) :
        camera(camera),
        scenario(scenario),
        mapWidth(scenario->getWidth()),
        mapHeight(scenario->getHeight()),
        entity(-1) {}

    void MousePicker::handleMouse() {

        // Get the mouse position relative to the window, in pixels
        int mouseX;
        int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Calculate the mouse position relative to the viewport, in pixels.
        // Since our viewport fills the window, no conversion is really
        // performed here.
        int viewportX = 0;
        int viewportY = 0;
        int viewportWidth = Rival::windowWidth;
        int viewportHeight = Rival::windowHeight;
        int mouseInViewportX = mouseX - viewportX;
        int mouseInViewportY = mouseY - viewportY;

        // Calculate mouse position relative to the viewport, in the range 0-1
        float normalizedMouseX =
            static_cast<float>(mouseInViewportX) / viewportWidth;
        float normalizedMouseY =
            static_cast<float>(mouseInViewportY) / viewportHeight;

        // Calculate the mouse position in world units
        float mouseWorldX = camera.getLeft()
            + normalizedMouseX * camera.getWidth();
        float mouseWorldY = camera.getTop()
            + normalizedMouseY * camera.getHeight();

        // Get the naive tile position.
        // This would work if our tiles were regular and aligned with the
        // camera's axes, but they are diamond shaped and positioned in a
        // zigzag pattern, so we will need to make further adjustments.
        tileX = static_cast<int>(mouseWorldX);
        tileY = static_cast<int>(mouseWorldY);

        // Get the "remainder" (just the decimal part of the world position).
        // Because of the way the tiles overlap, offsetX now ranges from 0-1,
        // where 0 is the left of a tile and 1 is the CENTRE of a tile; going
        // past the centre of a tile actually puts us in the next tile over
        // (tileX + 1). This means we are always considering only the LEFT side
        // of a tile. The meaning of offsetY is more complicated due to the
        // zigzagging tile positioning (keep reading).
        float offsetX = mouseWorldX - tileX;
        float offsetY = mouseWorldY - tileY;

        // Now that we know our naive tile co-ordinates and the mouse offset
        // within that tile, we can figure out the real tile. Imagine each tile
        // is divided into four quadrants. Since our tiles are diamonds, each
        // quadrant then contains a diagonal line; the border between our tile
        // and one of its neighbours. Based on the quadrant, and which side of
        // the line we are on, we can figure out what adjustments we need to
        // make to our naive tile co-ordinates.
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

        // Reset selected entity
        entity = -1;

        // Check for Units under the mouse
        if (scenario) {
            auto& units = scenario->getUnits();
            bool foundEntity = false;
            for (auto it = units.begin(); it != units.end(); ++it) {
                const Unit& unit = *it->second;
                if (isMouseInUnit(unit, mouseInViewportX, mouseInViewportY)) {
                    entity = unit.getId();
                    std::cout << "Entity " << entity << " is under cursor.\n";
                    break;
                }
            }
        }
    }

    bool MousePicker::isMouseInUnit(
            const Unit& unit, int mouseInViewportX, int mouseInViewportY) {

        /*
         * Given the tile co-ordinate of a Unit, we need to figure out the
         * position of that unit's hitbox.
         *
         * We work in pixels here because it we are concerned with the rendered
         * position of the unit.
         *
         * At a pixel-perfect scale, the bottom-left corner of the hitbox will
         * always be a fixed offset from the top-left of the tile, as all units
         * are positioned the same way relative to the tile they're in.
         * However, the height does vary for different units.
         *
         * We also have to consider the camera zoom level, since the hitbox
         * will appear bigger when we are more zoomed in.
         */

        // Find the position of the top-left corner of the unit's tile, in px
        float tileX_px = static_cast<float>(
                RenderUtils::tileToPx_X(unit.getX()));
        float tileY_px = static_cast<float>(
                RenderUtils::tileToPx_Y(unit.getX(), unit.getY()));

        // Adjust based on the camera position
        // (as the camera pans right, tiles are rendered further to the left!)
        tileX_px -= RenderUtils::worldToPx_X(camera.getLeft());
        tileY_px -= RenderUtils::worldToPx_Y(camera.getTop());

        // TODO: Adjust based on the camera zoom

        // Find the bottom-left corner of the unit hitbox
        float unitX1 = tileX_px + unitHitboxOffsetX;
        float unitX2 = unitX1 + unitHitboxWidth;
        float unitY2 = tileY_px + unitHitboxOffsetY;
        // TMP: For now we use a fixed height
        float unitY1 = unitY2 - unitHitboxHeight;

        std::cout << unitX1 << " < " << mouseInViewportX << " < " << unitX2 << "\n";

        // Finally, see if the mouse is inside the hitbox
        return mouseInViewportX >= unitX1
                && mouseInViewportY >= unitY1
                && mouseInViewportX < unitX2
                && mouseInViewportY < unitY2;
    }

    int MousePicker::getTileX() const {
        return tileX;
    }

    int MousePicker::getTileY() const {
        return tileY;
    }

    int MousePicker::getEntity() const {
        return entity;
    }
}
