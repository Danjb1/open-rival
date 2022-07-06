#include "pch.h"

#include "MouseHandlerComponent.h"

#include "Entity.h"

namespace Rival {

const std::string MouseHandlerComponent::key = "mouse_handler";

MouseHandlerComponent::MouseHandlerComponent()
    : EntityComponent(key)
{
}

Rect MouseHandlerComponent::getHitbox(float cameraX_px, float cameraY_px) const
{
    /*
     * Entities are always rendered at a fixed pixel offset from the tile
     * they occupy. Thus, if we can figure out the rendered position of
     * this tile sprite (A), we can figure out the rendered position of the
     * entity (B).
     *
     *               B=======+
     *               |       |
     *      A--------|       |--------+
     *      |        |       |        |
     *      |      # |       | #      |
     *      |   #    |       |    #   |
     *      |<       +=======+       >|
     *      |   #                 #   |
     *      |      #           #      |
     *      |         #     #         |
     *      |            #            |
     *      |                         |
     *      |                         |
     *      +-------------------------+
     *
     * Note that the rendered tile position that we calculate is the
     * position BEFORE viewport scaling. If the game is not rendered in a
     * pixel-perfect manner (i.e. if the camera size, in pixels, does not
     * match the viewport), then this will not work correctly.
     *
     * For now, we assume all entities take up only one tile, so this won't
     * work properly for buildings.
     */

    // Find the rendered position of the top-left corner of the entity's
    // tile, in pixels
    const MapNode& pos = entity->getPos();
    float tileX_px = static_cast<float>(RenderUtils::tileToPx_X(pos.x));
    float tileY_px = static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y));

    // Adjust based on the camera position
    // (as the camera pans right, tiles are rendered further to the left!)
    tileX_px -= cameraX_px;
    tileY_px -= cameraY_px;

    // Find the bottom-left corner of the entity's hitbox. This is the
    // easiest corner to find, since it is always a fixed offset from the
    // containing tile, regardless of the height of the entity (except
    // perhaps for flying units!).
    float x1 = tileX_px + (unitHitboxOffsetX);
    float y2 = tileY_px + (unitHitboxOffsetY);

    /*
    if (movement.isInProgress())
    {
        // Reverse-engineer the rendering code here.
        // If we know the position at the start and destination, and we
        // know the progress (0-1), finding the current position is trivial
        // TODO: Introduce methods to reduce duplication
        float progress = movement.getProgress();
        x += progress * dirX;
        y += progress * dirY;
    }
    */

    // Now we can trivially find the entity bounds
    // TMP: For now we use a fixed height for all entities
    float y1 = y2 - unitHitboxHeight;
    return { x1, y1, unitHitboxWidth, unitHitboxHeight };
}

bool MouseHandlerComponent::isSelectable() const
{
    // TODO: return false for walls, etc.
    return true;
}

void MouseHandlerComponent::onSelect()
{
    // TODO: Depends on state and entity type
    //
    // For friendly units:
    // - Play voice
    //
    // For enemy units or when in attack mode:
    // - Attack (depending on selection)
    //
    // For chests:
    // - Move there (depending on selection)
}

void MouseHandlerComponent::onTileClicked(const MapNode&)
{
    // TODO: Depends on state and entity type (e.g. move, harvest, cast spell)
    // TODO: What about when a group is selected?
}

}  // namespace Rival
