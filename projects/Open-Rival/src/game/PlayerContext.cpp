#include "game/PlayerContext.h"

namespace Rival {

void DragSelect::reset()
{
    startX = -1;
    startY = -1;
    endX = -1;
    endY = -1;
}

bool DragSelect::isActive() const
{
    return startX >= 0;
}

bool DragSelect::isValid() const
{
    return isActive()                                       //
            && std::abs(endX - startX) > minDragSelectSize  //
            && std::abs(endY - startY) > minDragSelectSize;
}

void PlayerContext::setCurrentAction(PlayerAction newAction)
{
    currentAction = newAction;

    if (newAction != PlayerAction::DragSelect)
    {
        dragSelect.reset();
    }
}

std::set<int> PlayerContext::getSelectedEntityIds() const
{
    std::set<int> entityIds;
    for (const auto& weakSelectedEntity : weakSelectedEntities)
    {
        const auto& selectedEntity = weakSelectedEntity.lock();
        if (selectedEntity)
        {
            entityIds.insert(selectedEntity->getId());
        }
    }
    return entityIds;
}

std::vector<int> PlayerContext::getSelectedEntityIdsVector() const
{
    std::set<int> entityIdsSet = getSelectedEntityIds();
    std::vector<int> entityIdsVector(entityIdsSet.cbegin(), entityIdsSet.cend());
    return entityIdsVector;
}

std::weak_ptr<Entity> PlayerContext::getFirstSelectedEntity() const
{
    if (weakSelectedEntities.empty())
    {
        return {};
    }
    return weakSelectedEntities[0];
}

}  // namespace Rival
