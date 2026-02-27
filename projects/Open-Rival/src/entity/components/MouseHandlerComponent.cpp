#include "entity/components/MouseHandlerComponent.h"

#include <glm/vec2.hpp>

#include <memory>
#include <set>

#include "commands/AttackCommand.h"
#include "commands/MoveCommand.h"
#include "entity/Entity.h"
#include "entity/components/AttackComponent.h"
#include "entity/components/MovementComponent.h"
#include "entity/components/OwnerComponent.h"
#include "entity/components/SpriteComponent.h"
#include "entity/components/VoiceComponent.h"
#include "game/PlayerContext.h"
#include "gfx/renderer/EntityRenderer.h"

namespace Rival {

const std::string MouseHandlerComponent::key = "mouse_handler";

MouseHandlerComponent::MouseHandlerComponent(int hitboxWidth, int hitboxHeight)
    : EntityComponent(key)
    , hitboxWidth(hitboxWidth)
    , hitboxHeight(hitboxHeight)
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

    weakAttackComponent = entity->getComponentWeak<AttackComponent>(AttackComponent::key);
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

    // Find the rendered position of the top-left corner of the entity's tile, in pixels
    const MapNode& pos = entity->getPos();
    float tileX_px = static_cast<float>(RenderUtils::tileToPx_X(pos.x));
    float tileY_px = static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y));

    // Now we can find the center-bottom of the entity's hitbox
    // (this point never changes, regardless of the hitbox size).
    float hitboxOriginX = tileX_px + (unitHitboxOffsetX);
    float hitboxOriginY = tileY_px + (unitHitboxOffsetY);

    // Add a y-offset for flying Units
    if (Unit* unit = entity->as<Unit>())
    {
        hitboxOriginY += unit->getOffsetY();
    }

    // Add the last lerp offset that was used to render the entity
    if (moving)
    {
        if (auto spriteComponent = weakSpriteComponent.lock())
        {
            glm::vec2 lerpOffset = spriteComponent->lastLerpOffset;
            hitboxOriginX += lerpOffset.x;
            hitboxOriginY += lerpOffset.y;
        }
    }

    return { //
        hitboxOriginX - static_cast<float>(hitboxWidth) / 2.f,
        hitboxOriginY - static_cast<float>(hitboxHeight),
        static_cast<float>(hitboxWidth),
        static_cast<float>(hitboxHeight)
    };
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
        /*
         * This is a little complicated.
         * Whether or not we play a voice clip here depends on the leader of the selection.
         * BUT, we need to issue an AttackCommand either way, because other entities in the selection might still be
         * able to attack us.
         */

        if (isEntityTargetableBySelection(playerContext))
        {
            // Play a sound (from a currently-selected unit)
            std::shared_ptr<Entity> selectedEntity = playerContext.getFirstSelectedEntity().lock();
            if (const auto voiceComponent = selectedEntity->getComponent<VoiceComponent>())
            {
                voiceComponent->playSound(UnitSoundType::Move);
            }
        }

        // Issue an AttackCommand
        std::vector<int> entityIds = playerContext.getSelectedEntityIdsVector();
        cmdInvoker.dispatchCommand(std::make_shared<AttackCommand>(entityIds, entity->getId()));
    }
}

bool MouseHandlerComponent::isEntityTargetableBySelection(const PlayerContext& playerContext) const
{
    std::shared_ptr<Entity> selectedEntity = playerContext.getFirstSelectedEntity().lock();
    if (const auto attackComponent = selectedEntity->getComponent<AttackComponent>())
    {
        return attackComponent->isValidTarget(entity->shared_from_this());
    }

    return false;
}

bool MouseHandlerComponent::onTileClicked(
        GameCommandInvoker& cmdInvoker, const PlayerStore& playerStore, const PlayerContext& playerContext)
{
    // Check owner
    const auto ownerComponent = weakOwnerComponent.lock();
    if (!ownerComponent || !playerStore.isLocalPlayer(ownerComponent->getPlayerId()))
    {
        // Other players' units cannot be controlled
        return false;
    }

    PlayerAction currentAction = playerContext.getCurrentAction();

    // TODO: Handle other actions (e.g. harvest, cast spell)
    if (currentAction == PlayerAction::Attack)
    {
        return attackTile(cmdInvoker, playerContext);
    }

    return moveToTile(cmdInvoker, playerContext);
}

bool MouseHandlerComponent::attackTile(GameCommandInvoker& cmdInvoker, const PlayerContext& playerContext)
{
    // Command selected entities to attack
    if (auto attackComponent = weakAttackComponent.lock())
    {
        // Play a sound
        if (const auto voiceComponent = weakVoiceComponent.lock())
        {
            voiceComponent->playSound(UnitSoundType::Move);
        }

        // Issue the AttackCommand
        std::vector<int> entityIds = playerContext.getSelectedEntityIdsVector();
        cmdInvoker.dispatchCommand(std::make_shared<AttackCommand>(entityIds, playerContext.tileUnderMouse));
        return true;
    }

    return false;
}

bool MouseHandlerComponent::moveToTile(GameCommandInvoker& cmdInvoker, const PlayerContext& playerContext)
{
    // Command selected entities to move
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

}  // namespace Rival
