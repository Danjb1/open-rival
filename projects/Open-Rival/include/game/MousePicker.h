#pragma once

#include <cstdint>
#include <memory>
#include <utility>

#include "entity/components/HealthComponent.h"
#include "game/MapUtils.h"
#include "utils/EntityUtils.h"

namespace Rival {

class Camera;
class Entity;
class GameCommandInvoker;
class PlayerStore;
class Rect;
class World;
struct PlayerContext;

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
    : public std::enable_shared_from_this<MousePicker>
    , public HealthListener
{
public:
    MousePicker(Camera& camera,
            Rect& viewport,
            World& world,
            PlayerContext& playerContext,
            PlayerStore& playerStore,
            GameCommandInvoker& cmdInvoker);

    virtual ~MousePicker() = default;

    void mouseDown(std::uint8_t button);
    void mouseUp(std::uint8_t button);
    void handleMouse();

    // Begin HealthListener override
    void onHealthChanged(Entity* entity, int prevValue, int newValue) override;
    void onMaxHealthChanged(Entity* entity, int prevValue, int newValue) override;
    void onHealthDepleted(Entity* entity) override;
    // Begin HealthListener override

private:
    /** Gets a weak pointer to this MousePicker.
     * NOTE: This requires that this MousePicker was created using std::make_shared. */
    std::weak_ptr<MousePicker> getWeakThis();

    float getMouseInCameraX(float normalizedMouseX);
    float getMouseInCameraY(float normalizedMouseY);

    MapNode getTilePos(float mouseWorldX, float mouseWorldY);

    std::weak_ptr<Entity> findEntityUnderMouse(int mouseInViewportX, int mouseInViewportY) const;
    WeakMutableEntityList findEntitiesForDragSelect(const Rect& area) const;

    void selectEntities(WeakMutableEntityList& entities);
    void deselect();

    void onEntityClicked(std::shared_ptr<Entity> clickedEntity);
    void onTileClicked();

    void processDragSelectArea();

private:
    World& world;
    Camera& camera;
    Rect& viewport;
    PlayerContext& playerContext;
    PlayerStore& playerStore;
    GameCommandInvoker& cmdInvoker;

    int mapWidth;
    int mapHeight;
};

}  // namespace Rival
