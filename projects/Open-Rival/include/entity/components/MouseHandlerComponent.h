#pragma once

#include <string>

#include "commands/GameCommand.h"
#include "entity/components/EntityComponent.h"
#include "entity/components/MovementListener.h"
#include "game/PlayerState.h"
#include "gfx/RenderUtils.h"
#include "utils/Rect.h"

namespace Rival {

class AttackComponent;
class Camera;
class MovementComponent;
class OwnerComponent;
class SpriteComponent;
class VoiceComponent;
struct MapNode;
struct PlayerContext;

/**
 * Component that allows an Entity to respond to mouse input.
 */
class MouseHandlerComponent
    : public EntityComponent
    , public MovementListener
{
public:
    MouseHandlerComponent(int hitboxWidth, int hitboxHeight);

    // Begin EntityComponent override
    virtual void onEntityFirstAddedToWorld(World* world) override;
    virtual void destroy() override;
    // End EntityComponent override

    // Begin MovementComponent override
    virtual void onUnitMoveStart(const MapNode* nextNode) override;
    virtual void onUnitPaused() override;
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
     *
     * This could be called as a result of the entity being clicked directly, or a drag-select. In the case of a
     * drag-select, isLeader will be true for only 1 unit.
     */
    void onSelected(const PlayerStore& playerStore, bool isLeader);

    /**
     * Called when this Entity is targeted by an action.
     */
    void onTargeted(GameCommandInvoker& cmdInvoker, const PlayerStore& playerStore, const PlayerContext& playerContext);

    /**
     * Called when a tile is clicked, with this Entity selected.
     *
     * If a group is selected, this is only called for the group leader.
     */
    bool onTileClicked(
            GameCommandInvoker& cmdInvoker, const PlayerStore& playerStore, const PlayerContext& playerContext);

private:
    const Rect createHitbox() const;

    bool attackTile(GameCommandInvoker& cmdInvoker, const PlayerContext& playerContext);
    bool moveToTile(GameCommandInvoker& cmdInvoker, const PlayerContext& playerContext);

public:
    static const std::string key;

private:
    // Offset of a Unit's hitbox, measured from the top-left corner of the containing tile to the center of the bottom
    // edge of the hitbox, in px.
    static constexpr int unitHitboxOffsetX = 31;
    static constexpr int unitHitboxOffsetY = 22;

    std::weak_ptr<MovementComponent> weakMovementComponent;
    std::weak_ptr<AttackComponent> weakAttackComponent;
    std::weak_ptr<OwnerComponent> weakOwnerComponent;
    std::weak_ptr<SpriteComponent> weakSpriteComponent;
    std::weak_ptr<VoiceComponent> weakVoiceComponent;

    mutable Rect hitbox;
    int hitboxWidth;
    int hitboxHeight;

    mutable bool dirty = true;
    bool moving = true;
};

}  // namespace Rival
