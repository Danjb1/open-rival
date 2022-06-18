#pragma once

#include <memory>
#include <utility>

#include "RenderUtils.h"

namespace Rival {

class Camera;
class Entity;
class Rect;
class World;

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
class MousePicker
{

public:
    MousePicker(Camera& camera, Rect& viewport, World& world);

    void handleMouse();

    int getTileX() const;

    int getTileY() const;

    int getEntityId() const;

private:
    // Offset of a Unit's hitbox, measured from the top-left corner of the
    // containing tile to the bottom-left of the hitbox, in px
    static constexpr int unitHitboxOffsetX = 20;
    static constexpr int unitHitboxOffsetY = 25;

    // Size of a Unit's hitbox
    static constexpr int unitHitboxWidth = RenderUtils::tileWidthPx - (2 * unitHitboxOffsetX);
    static constexpr int unitHitboxHeight = 40;

    Camera& camera;

    Rect& viewport;

    int mapWidth;

    int mapHeight;

    std::pair<int, int> tile;

    int entityId;

    World& world;

    float getMouseInCameraX(float normalizedMouseX);
    float getMouseInCameraY(float normalizedMouseY);

    std::pair<int, int> getTilePos(float mouseWorldX, float mouseWorldY);

    void findEntityUnderMouse(int mouseInViewportX, int mouseInViewportY);

    bool isMouseInEntity(const Entity& entity, int mouseInViewportX, int mouseInViewportY);
};

}  // namespace Rival
