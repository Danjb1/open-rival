#include "ui/Cursor.h"

#include "entity/components/MovementComponent.h"
#include "entity/components/OwnerComponent.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"

namespace Rival {

CursorDef Cursor::getCurrentCursor(const PlayerStore& playerStore, const PlayerContext& playerContext)
{
    // Drag-select
    const PlayerAction currentMode = playerContext.getCurrentAction();
    if (currentMode == PlayerAction::DragSelect)
    {
        return dragSelect;
    }

    // See if we have any units selected.
    // We only care about 1 of the selected entities; doesn't matter which.
    const auto selectedEntity = playerContext.getFirstSelectedEntity().lock();

    // If an entity is hovered, show attack/harvest/whatever cursor (based on context)
    const auto entityUnderMouse = playerContext.weakEntityUnderMouse.lock();
    if (entityUnderMouse)
    {
        const OwnerComponent* selectedOwnerComp =
                selectedEntity ? selectedEntity->getComponent<OwnerComponent>() : nullptr;
        const bool isOwnEntitySelected =
                selectedOwnerComp && playerStore.isLocalPlayer(selectedOwnerComp->getPlayerId());

        const OwnerComponent* targetOwnerComp = entityUnderMouse->getComponent<OwnerComponent>();
        const bool isEnemyHovered =
                isOwnEntitySelected && targetOwnerComp && !playerStore.isSameTeam(targetOwnerComp->getPlayerId());
        if (isEnemyHovered || currentMode == PlayerAction::Attack)
        {
            return attack;
        }

        return select;
    }

    // If we are in attack mode then we failed to find a target by this point
    if (currentMode == PlayerAction::Attack)
    {
        return targetInvalid;
    }

    // If we have an entity selected, see if we can move
    if (selectedEntity)
    {
        const auto owner = selectedEntity->getComponent<OwnerComponent>(OwnerComponent::key);
        const auto movement = selectedEntity->getComponent<MovementComponent>(MovementComponent::key);

        if (owner && movement && playerStore.isLocalPlayer(owner->getPlayerId()))
        {
            return move;
        }
    }

    // Default cursor
    return arrow;
}

}  // namespace Rival
