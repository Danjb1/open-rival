#pragma once

#include <string>

#include "EntityComponent.h"
#include "GameCommand.h"
#include "MovementComponent.h"
#include "PlayerState.h"
#include "Rect.h"
#include "RenderUtils.h"

namespace Rival {

class Camera;
class OwnerComponent;
class SpriteComponent;
class VoiceComponent;
struct MapNode;

/**
 * Component that allows an Entity to respond to mouse input.
 */
class MouseHandlerComponent
    : public EntityComponent
    , public MovementListener
{
public:
    MouseHandlerComponent();

    // Begin EntityComponent override
    virtual void onEntitySpawned(World* world) override;
    virtual void onDelete() override;
    // End EntityComponent override

    // Begin MovementComponent override
    virtual void onUnitMoveStart(const MapNode* nextNode) override;
    virtual void onUnitStopped() override;
    // End MovementComponent override

    /**
     * Gets the Entity's hitbox, in pixels, assuming a pixel-perfect game world.
     */
    const Rect& getHitbox() const;

    /**
     * Determines if this Entity can be selected.
     */
    bool isSelectable() const;

    /**
     * Called when this Entity is selected.
     */
    void onSelect(const PlayerStore& playerStore);

    /**
     * Called when a tile is clicked, with this Entity selected.
     */
    void onTileClicked(GameCommandInvoker& cmdInvoker, const PlayerStore& playerStore, const MapNode& tilePos);

private:
    const Rect createHitbox() const;

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

    std::weak_ptr<MovementComponent> weakMovementComponent;
    std::weak_ptr<OwnerComponent> weakOwnerComponent;
    std::weak_ptr<SpriteComponent> weakSpriteComponent;
    std::weak_ptr<VoiceComponent> weakVoiceComponent;

    mutable Rect hitbox;

    mutable bool dirty = true;
    bool moving = true;
};

}  // namespace Rival
