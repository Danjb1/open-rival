#pragma once

#include <string>

#include "EntityComponent.h"
#include "Rect.h"
#include "RenderUtils.h"

namespace Rival {

class Camera;
struct MapNode;

/**
 * Component that allows an Entity to respond to mouse input.
 */
class MouseHandlerComponent : public EntityComponent
{
public:
    MouseHandlerComponent();

    /**
     * Gets the Entity's hitbox in the rendered game world, in pixels, using the given camera position.
     *
     * This does not take into account the camera zoom, that is, it assumes a pixel-perfect game world.
     */
    Rect getHitbox(float cameraX_px, float cameraY_px) const;

    /**
     * Determines if this Entity can be selected.
     */
    bool isSelectable() const;

    /**
     * Called when this Entity is selected.
     */
    void onSelect();

    /**
     * Called when a tile is clicked, with this Entity selected.
     */
    void onTileClicked(const MapNode& tilePos);

    /*
    Cursor getHoverCursor()
    {
        // TODO: Depends on state and entity type
    }
    */

public:
    static const std::string key;

private:
    // Offset of a Unit's hitbox, measured from the top-left corner of the
    // containing tile to the bottom-left of the hitbox, in px
    static constexpr int unitHitboxOffsetX = 20;
    static constexpr int unitHitboxOffsetY = 25;

    // Size of a Unit's hitbox
    static constexpr float unitHitboxWidth = RenderUtils::tileWidthPx - (2 * unitHitboxOffsetX);
    static constexpr float unitHitboxHeight = 40.f;
};

}  // namespace Rival
