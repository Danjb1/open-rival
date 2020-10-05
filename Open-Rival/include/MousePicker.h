#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include <memory>
#include <utility>

#include "Camera.h"
#include "Rect.h"
#include "RenderUtils.h"
#include "Unit.h"

namespace Rival {

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
        // tile it's in, in px
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

        float getMouseInCameraX(int mouseInViewportX);
        float getMouseInCameraY(int mouseInViewportX);

        std::pair<int, int> getTilePos(float mouseWorldX, float mouseWorldY);

        void findEntityUnderMouse(int mouseInViewportX, int mouseInViewportY);

        bool isMouseInEntity(
                const Entity& entity,
                int mouseInViewportX,
                int mouseInViewportY);
    };

}  // namespace Rival

#endif  // MOUSE_PICKER_H
