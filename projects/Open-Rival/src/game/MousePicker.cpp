#include "game/MousePicker.h"

#include "utils/SDLWrapper.h"

#include <algorithm>  // min, max
#include <cstdint>
#include <cstdlib>  // abs

#include "commands/GameCommand.h"
#include "entity/Entity.h"
#include "entity/components/MouseHandlerComponent.h"
#include "entity/components/MovementComponent.h"
#include "entity/components/OwnerComponent.h"
#include "game/MapUtils.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"
#include "game/World.h"
#include "gfx/Camera.h"
#include "gfx/RenderUtils.h"
#include "utils/LogUtils.h"
#include "utils/MathUtils.h"
#include "utils/MouseUtils.h"
#include "utils/Rect.h"

namespace Rival {

MousePicker::MousePicker(Camera& camera,
        Rect& viewport,
        World& world,
        PlayerContext& playerContext,
        PlayerStore& playerStore,
        GameCommandInvoker& cmdInvoker)
    : camera(camera)
    , viewport(viewport)
    , world(world)
    , playerContext(playerContext)
    , playerStore(playerStore)
    , cmdInvoker(cmdInvoker)
    , mapWidth(world.getWidth())
    , mapHeight(world.getHeight())
{
}

std::weak_ptr<MousePicker> MousePicker::getWeakThis()
{
    return std::dynamic_pointer_cast<MousePicker>(shared_from_this());
}

void MousePicker::mouseDown(std::uint8_t button)
{
    if (button != SDL_BUTTON_LEFT)
    {
        return;
    }

    // Get the mouse position relative to the window, in pixels
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Abort if the mouse is outside the viewport
    if (!viewport.contains(mouseX, mouseY))
    {
        return;
    }

    if (playerContext.getCurrentAction() == PlayerAction::None)
    {
        // Start drag-select
        playerContext.dragSelect.startX = mouseX;
        playerContext.dragSelect.startY = mouseY;
        playerContext.dragSelect.endX = mouseX;
        playerContext.dragSelect.endY = mouseY;
    }
}

void MousePicker::mouseUp(std::uint8_t button)
{
    if (button == SDL_BUTTON_LEFT)
    {
        // Drag-select
        if (playerContext.getCurrentAction() == PlayerAction::DragSelect)
        {
            processDragSelectArea();
            playerContext.setCurrentAction(PlayerAction::None);
            return;
        }

        // Get the mouse position relative to the window, in pixels
        int mouseX;
        int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Abort if the mouse is outside the viewport
        if (!viewport.contains(mouseX, mouseY))
        {
            playerContext.setCurrentAction(PlayerAction::None);
            return;
        }

        // Single click
        if (const auto entityUnderMouse = playerContext.weakEntityUnderMouse.lock())
        {
            onEntityClicked(entityUnderMouse);
        }
        else
        {
            onTileClicked();
        }

        playerContext.setCurrentAction(PlayerAction::None);
    }
    else if (button == SDL_BUTTON_MIDDLE)
    {
        LOG_DEBUG("tile at {}, {} has passability: {}",
                playerContext.tileUnderMouse.x,
                playerContext.tileUnderMouse.y,
                EnumUtils::toIntegral(world.getPassability(playerContext.tileUnderMouse)));
    }
    else if (button == SDL_BUTTON_RIGHT)
    {
        deselect();
        playerContext.setCurrentAction(PlayerAction::None);
    }
}

void MousePicker::handleMouse()
{
    // Get the mouse position relative to the window, in pixels
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Drag-select
    if (playerContext.dragSelect.isActive())
    {
        // Don't allow the drag-select area to extend outside the viewport
        playerContext.dragSelect.endX =
                std::clamp(mouseX, static_cast<int>(viewport.x), static_cast<int>(viewport.x + viewport.width));
        playerContext.dragSelect.endY =
                std::clamp(mouseY, static_cast<int>(viewport.y), static_cast<int>(viewport.y + viewport.height));

        if (playerContext.dragSelect.isValid())
        {
            playerContext.setCurrentAction(PlayerAction::DragSelect);
        }

        return;
    }

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
    playerContext.tileUnderMouse = getTilePos(mouseCameraX, mouseCameraY);
    playerContext.weakEntityUnderMouse = findEntityUnderMouse(mouseInViewportX, mouseInViewportY);
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

std::weak_ptr<Entity> MousePicker::findEntityUnderMouse(int mouseInViewportX, int mouseInViewportY) const
{
    // Find the camera position, in pixels
    float cameraX_px = RenderUtils::cameraToPx_X(camera.getLeft());
    float cameraY_px = RenderUtils::cameraToPx_Y(camera.getTop());

    // Find the mouse position in the world by reversing the camera transform
    float zoom = camera.getZoom();
    float mouseInWorldX = cameraX_px + (mouseInViewportX / zoom);
    float mouseInWorldY = cameraY_px + (mouseInViewportY / zoom);

    // TODO: We could optimise this by considering only Entities that were rendered in the previous frame.
    const auto entities = world.getMutableEntities();
    for (const auto& e : entities)
    {
        MouseHandlerComponent* mouseHandlerComponent =
                e->getComponent<MouseHandlerComponent>(MouseHandlerComponent::key);
        if (!mouseHandlerComponent)
        {
            continue;
        }

        const Rect& hitbox = mouseHandlerComponent->getHitbox();
        if (hitbox.contains(mouseInWorldX, mouseInWorldY))
        {
            return e;
        }
    }

    return {};
}

WeakMutableEntityList MousePicker::findEntitiesForDragSelect(const Rect& area) const
{
    WeakMutableEntityList entitiesInArea;

    // TODO: We could optimise this by considering only Entities that were rendered in the previous frame.
    const auto entities = world.getMutableEntities();
    for (const auto& e : entities)
    {
        const auto& mouseHandlerComponent = e->getComponent<MouseHandlerComponent>(MouseHandlerComponent::key);
        if (!mouseHandlerComponent)
        {
            continue;
        }

        const auto& owner = e->getComponent<OwnerComponent>(OwnerComponent::key);
        if (!owner || !playerStore.isLocalPlayer(owner->getPlayerId()))
        {
            // Only locally-owned entities can be selected by drag-select
            continue;
        }

        const Rect& hitbox = mouseHandlerComponent->getHitbox();
        if (area.intersects(hitbox))
        {
            entitiesInArea.push_back(e);
        }
    }

    return entitiesInArea;
}

void MousePicker::selectEntities(WeakMutableEntityList& entities)
{
    // Clean up the old selection
    for (const auto& weakSelectedEntity : playerContext.weakSelectedEntities)
    {
        const auto& selectedEntity = weakSelectedEntity.lock();
        if (!selectedEntity)
        {
            // Selected entity no longer exists
            continue;
        }

        if (const auto healthComponent = selectedEntity->getComponent<HealthComponent>())
        {
            healthComponent->removeListener(getWeakThis());
        }
    }

    playerContext.weakSelectedEntities = entities;
    bool isLeader = true;

    // Prepare the new selection
    for (const auto& weakSelectedEntity : playerContext.weakSelectedEntities)
    {
        const auto& selectedEntity = weakSelectedEntity.lock();
        if (!selectedEntity)
        {
            // Selected entity no longer exists (should never happen since they've just been selected!)
            continue;
        }

        if (const auto mouseHandlerComponent = selectedEntity->getComponent<MouseHandlerComponent>())
        {
            mouseHandlerComponent->onSelected(playerStore, isLeader);
        }

        if (const auto healthComponent = selectedEntity->getComponent<HealthComponent>())
        {
            healthComponent->addListener(getWeakThis());
        }

        // TODO: Also listen for when entity is removed from world (e.g. boarding a vehicle)

        // TMP: For now, the first unit in the selection is the leader
        isLeader = false;
    }
}

void MousePicker::deselect()
{
    playerContext.weakSelectedEntities.clear();
}

void MousePicker::onEntityClicked(std::shared_ptr<Entity> clickedEntity)
{
    std::shared_ptr<Entity> selectedEntity = playerContext.getFirstSelectedEntity().lock();
    const auto selectedOwnerComp = selectedEntity ? selectedEntity->getComponent<OwnerComponent>() : nullptr;
    const bool isOwnUnitSelected = selectedOwnerComp && playerStore.isLocalPlayer(selectedOwnerComp->getPlayerId());

    const auto targetOwnerComp = clickedEntity->getComponent<OwnerComponent>();
    const bool wasOwnUnitClicked = targetOwnerComp && playerStore.isLocalPlayer(targetOwnerComp->getPlayerId());

    // We can only perform an action if we have our own unit selected already
    bool isPerformingAction = isOwnUnitSelected;

    if (wasOwnUnitClicked && playerContext.getCurrentAction() == PlayerAction::None)
    {
        // We are just clicking from one unit to another
        isPerformingAction = false;
    }

    if (isPerformingAction)
    {
        if (const auto mouseHandlerComponent =
                        clickedEntity->getComponent<MouseHandlerComponent>(MouseHandlerComponent::key))
        {
            mouseHandlerComponent->onTargeted(cmdInvoker, playerStore, playerContext);
        }
    }
    else
    {
        // Select the clicked entity
        WeakMutableEntityList entityAsList = { playerContext.weakEntityUnderMouse };
        selectEntities(entityAsList);
    }
}

void MousePicker::onTileClicked()
{
    // Inform the first entity of the input
    for (const auto& weakSelectedEntity : playerContext.weakSelectedEntities)
    {
        const auto& selectedEntity = weakSelectedEntity.lock();
        if (!selectedEntity)
        {
            // Selected entity no longer exists
            continue;
        }

        if (const auto& mouseHandlerComponent =
                        selectedEntity->getComponent<MouseHandlerComponent>(MouseHandlerComponent::key))
        {
            bool inputHandled = mouseHandlerComponent->onTileClicked(cmdInvoker, playerStore, playerContext);
            if (inputHandled)
            {
                break;
            }
        }
    }
}

void MousePicker::processDragSelectArea()
{
    // Normalize the drag-select area
    const auto& dragSelect = playerContext.dragSelect;
    int startX = std::min(dragSelect.startX, dragSelect.endX);
    int startY = std::min(dragSelect.startY, dragSelect.endY);
    int endX = std::max(dragSelect.startX, dragSelect.endX);
    int endY = std::max(dragSelect.startY, dragSelect.endY);

    // Find the camera position, in pixels
    float cameraX_px = RenderUtils::cameraToPx_X(camera.getLeft());
    float cameraY_px = RenderUtils::cameraToPx_Y(camera.getTop());

    // Convert drag-select area to world units by reversing the camera transform
    float zoom = camera.getZoom();
    float startXWorld = (startX / zoom) + cameraX_px;
    float startYWorld = (startY / zoom) + cameraY_px;
    float endXWorld = (endX / zoom) + cameraX_px;
    float endYWorld = (endY / zoom) + cameraY_px;

    float width = endXWorld - startXWorld;
    float height = endYWorld - startYWorld;
    auto entitiesInArea = findEntitiesForDragSelect({ startXWorld, startYWorld, width, height });
    selectEntities(entitiesInArea);
}

void MousePicker::onHealthChanged(Entity* /*entity*/, int /*prevValue*/, int /*newValue*/)
{
    // Nothing to do
}

void MousePicker::onMaxHealthChanged(Entity* /*entity*/, int /*prevValue*/, int /*newValue*/)
{
    // Nothing to do
}

void MousePicker::onHealthDepleted(Entity* entity)
{
    playerContext.deselectEntity(entity->getId());
}

}  // namespace Rival
