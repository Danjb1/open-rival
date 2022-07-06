#pragma once

#include <memory>
#include <utility>

#include "MapUtils.h"
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

    void mouseDown();
    void mouseUp();
    void handleMouse();

    MapNode getTilePos() const;

private:
    float getMouseInCameraX(float normalizedMouseX);
    float getMouseInCameraY(float normalizedMouseY);

    MapNode getTilePos(float mouseWorldX, float mouseWorldY);

    std::weak_ptr<Entity> findEntityUnderMouse(int mouseInViewportX, int mouseInViewportY);

    void entitySelected(std::shared_ptr<Entity> entity);
    void tileSelected();

private:
    Camera& camera;

    Rect& viewport;

    int mapWidth;

    int mapHeight;

    MapNode tileUnderMouse;

    std::weak_ptr<Entity> weakEntityUnderMouse;
    std::weak_ptr<Entity> weakSelectedEntity;

    World& world;
};

}  // namespace Rival
