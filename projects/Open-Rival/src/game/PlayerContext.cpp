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

}  // namespace Rival
