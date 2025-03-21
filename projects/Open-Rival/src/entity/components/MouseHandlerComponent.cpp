#include "entity/components/MouseHandlerComponent.h"

#include <glm/vec2.hpp>

#include <memory>
#include <set>

#include "commands/AttackCommand.h"
#include "commands/MoveCommand.h"
#include "entity/Entity.h"
#include "entity/components/MovementComponent.h"
#include "entity/components/OwnerComponent.h"
#include "entity/components/SpriteComponent.h"
#include "entity/components/VoiceComponent.h"
#include "game/PlayerContext.h"
#include "gfx/renderer/EntityRenderer.h"

namespace Rival {

const std::string MouseHandlerComponent::key = "mouse_handler";

MouseHandlerComponent::MouseHandlerComponent()
    : EntityComponent(key)
{
}

void MouseHandlerComponent::onEntityFirstAddedToWorld(World*)
{
    weakMovementComponent = entity->requireComponentWeak<MovementComponent>(MovementComponent::key);
    if (auto movementComponent = weakMovementComponent.lock())
    {
        std::weak_ptr<MouseHandlerComponent> weakThis = entity->getComponentWeak<MouseHandlerComponent>(key);
        movementComponent->addListener(weakThis);
    }

    weakOwnerComponent = entity->getComponentWeak<OwnerComponent>(OwnerComponent::key);
    weakSpriteComponent = entity->getComponentWeak<SpriteComponent>(SpriteComponent::key);
    weakVoiceComponent = entity->getComponentWeak<VoiceComponent>(VoiceComponent::key);
}

void MouseHandlerComponent::destroy()
{
    if (auto movementComponent = weakMovementComponent.lock())
    {
        std::weak_ptr<MouseHandlerComponent> weakThis = entity->getComponentWeak<MouseHandlerComponent>(key);
        movementComponent->removeListener(weakThis);
    }
}

void MouseHandlerComponent::onUnitMoveStart(const MapNode*)
{
    dirty = true;
    moving = true;
}

void MouseHandlerComponent::onUnitPaused()
{
    // Do nothing
}

void MouseHandlerComponent::onUnitStopped()
{
    dirty = true;
    moving = false;
}

const Rect& MouseHandlerComponent::getHitbox() const
{
    if (dirty || moving)
    {
        hitbox = createHitbox();
        dirty = false;
    }

    return hitbox;
}

bool MouseHandlerComponent::isSelectable() const
{
    // TODO: return false for walls, etc.
    return true;
}

void MouseHandlerComponent::onSelected(const PlayerStore& playerStore, bool isLeader)
{
    const auto ownerComponent = weakOwnerComponent.lock();
    const bool isEnemy = ownerComponent && !playerStore.isSameTeam(ownerComponent->getPlayerId());
    if (isEnemy)
    {
        // Do nothing when an enemy unit is selected
        return;
    }

    // Leader should play a sound when selected
    if (isLeader)
    {
        const auto voiceComponent = weakVoiceComponent.lock();
        if (voiceComponent)
        {
            voiceComponent->playSound(UnitSoundType::Select);
        }
    }
}

void MouseHandlerComponent::onTargeted(
        GameCommandInvoker& cmdInvoker, const PlayerStore& playerStore, const PlayerContext& playerContext)
{
    PlayerAction actionToPerform = playerContext.getCurrentAction();

    if (actionToPerform == PlayerAction::None)
    {
        // No explicit action, but it can be inferred from the context
        const auto ownerComponent = weakOwnerComponent.lock();
        const bool isEnemy = ownerComponent && !playerStore.isSameTeam(ownerComponent->getPlayerId());
        if (isEnemy)
        {
            actionToPerform = PlayerAction::Attack;
        }
    }

    if (actionToPerform == PlayerAction::Attack)
    {
        // Play a sound (from a currently-selected unit)
        std::shared_ptr<Entity> selectedEntity = playerContext.getFirstSelectedEntity().lock();
        if (const auto voiceComponent = selectedEntity->getComponent<VoiceComponent>())
        {
            voiceComponent->playSound(UnitSoundType::Move);
        }

        // Issue an AttackCommand
        std::vector<int> entityIds = playerContext.getSelectedEntityIdsVector();
        cmdInvoker.dispatchCommand(std::make_shared<AttackCommand>(entityIds, entity->getId()));
    }
}

bool MouseHandlerComponent::onTileClicked(
        GameCommandInvoker& cmdInvoker, const PlayerStore& playerStore, const PlayerContext& playerContext)
{
    // TODO: Depends on state and entity type (e.g. move, harvest, cast spell)

    // Check owner
    const auto ownerComponent = weakOwnerComponent.lock();
    if (!ownerComponent || !playerStore.isLocalPlayer(ownerComponent->getPlayerId()))
    {
        // Other players' units cannot be controlled
        return false;
    }

    // Move all selected entities
    if (auto moveComponent = weakMovementComponent.lock())
    {
        // Play a sound
        if (const auto voiceComponent = weakVoiceComponent.lock())
        {
            voiceComponent->playSound(UnitSoundType::Move);
        }

        // Issue the MoveCommand
        std::vector<int> entityIds = playerContext.getSelectedEntityIdsVector();
        cmdInvoker.dispatchCommand(std::make_shared<MoveCommand>(entityIds, playerContext.tileUnderMouse));
        return true;
    }

    return false;
}

const Rect MouseHandlerComponent::createHitbox() const
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

    // Find the bottom-left corner of the entity's hitbox. This is the
    // easiest corner to find, since it is always a fixed offset from the
    // containing tile, regardless of the height of the entity (except
    // perhaps for flying units!).
    float x1 = tileX_px + (unitHitboxOffsetX);
    float y2 = tileY_px + (unitHitboxOffsetY);

    // Now we can find the top-left corner
    // TMP: For now we use a fixed height for all entities
    float y1 = y2 - unitHitboxHeight;

    // Add the last lerp offset that was used to render the entity
    if (moving)
    {
        if (auto spriteComponent = weakSpriteComponent.lock())
        {
            glm::vec2 lerpOffset = spriteComponent->lastLerpOffset;
            x1 += lerpOffset.x;
            y1 += lerpOffset.y;
        }
    }

    return { x1, y1, unitHitboxWidth, unitHitboxHeight };
}

}  // namespace Rival
