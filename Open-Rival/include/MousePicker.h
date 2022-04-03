#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include <memory>
#include <utility>

#include "Camera.h"
#include "Entity.h"
#include "Rect.h"
#include "RenderUtils.h"
#include "Unit.h"

namespace Rival {

    /**
     * Class responsible for determining what is under the mouse.
     *
     * Currently this works by reverse-engineering the rendering process and
     * calculating what was rendered at the mouse's position.
     *
     * As an alternative, we could use this strategy:
     * https://www.kamremake.com/devblog/unit-picking/
     *
     * This would be much simpler to understand and maintain, but it would
     * require everything to be rendered twice (once to a texture, using a
     * special mouse-picking shader that encodes information about the rendered
     * game world).
     */
    class MousePicker {

    public:
        MousePicker(
                Camera& camera,
                Rect& viewport,
                Scenario& scenario);

        void handleMouse();

        int getTileX() const;

        int getTileY() const;

        int getEntityId() const;

    private:
        // Offset of a Unit's hitbox, measured from the top-left corner of the
        // containing tile to the bottom-left of the hitbox, in px
        static const int unitHitboxOffsetX = 20;
        static const int unitHitboxOffsetY = 25;

        // Size of a Unit's hitbox
        static const int unitHitboxWidth = RenderUtils::tileWidthPx
                - (2 * unitHitboxOffsetX);
        static const int unitHitboxHeight = 40;

        Camera& camera;

        Rect& viewport;

        int mapWidth;

        int mapHeight;

        std::pair<int, int> tile;

        int entityId;

        Scenario& scenario;

        float getMouseInCameraX(float normalizedMouseX);
        float getMouseInCameraY(float normalizedMouseY);

        std::pair<int, int> getTilePos(float mouseWorldX, float mouseWorldY);

        void findEntityUnderMouse(int mouseInViewportX, int mouseInViewportY);

        bool isMouseInEntity(
                const Entity& entity,
                int mouseInViewportX,
                int mouseInViewportY);
    };

}  // namespace Rival

#endif  // MOUSE_PICKER_H
