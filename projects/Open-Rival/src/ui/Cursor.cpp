#include "ui/Cursor.h"

#include "entity/components/MovementComponent.h"
#include "entity/components/OwnerComponent.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"

namespace Rival {

CursorDef Cursor::getCurrentCursor(const PlayerStore& playerStore, const PlayerContext& playerContext)
{
    if (playerContext.dragSelect.isValid())
    {
        return dragSelect;
    }

    // We only care about 1 of the selected Entities, doesn't matter which
    const auto selectedEntity =
            playerContext.weakSelectedEntities.empty() ? nullptr : playerContext.weakSelectedEntities[0].lock();
    const auto entityUnderMouse = playerContext.weakEntityUnderMouse.lock();

    if (entityUnderMouse)
    {
        // TODO: If an entity is selected, show attack/harvest/whatever cursor here based on context
        return select;
    }

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
