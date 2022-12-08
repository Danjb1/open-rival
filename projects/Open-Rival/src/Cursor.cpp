#include "pch.h"

#include "Cursor.h"

#include "MovementComponent.h"
#include "OwnerComponent.h"
#include "PlayerContext.h"
#include "PlayerState.h"

namespace Rival {

CursorDef Cursor::getCurrentCursor(const PlayerStore& playerStore, const PlayerContext& playerContext)
{
    const auto selectedEntity = playerContext.weakSelectedEntity.lock();
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
