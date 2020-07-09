#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include <memory>

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
                std::shared_ptr<Scenario> scenario);

        void handleMouse();

        int getTileX() const;

        int getTileY() const;

        int getEntity() const;

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

        int tileX;

        int tileY;

        int entity;

        std::shared_ptr<Scenario> scenario;

        bool isMouseInUnit(
                const Unit& unit, int mouseInViewportX, int mouseInViewportY);
    };

}  // namespace Rival

#endif  // MOUSE_PICKER_H
