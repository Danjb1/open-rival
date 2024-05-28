#pragma once

#include <memory>
#include <set>

#include "entity/Entity.h"
#include "game/MapUtils.h"
#include "utils/EntityUtils.h"

namespace Rival {

struct DragSelect
{
    /** Minimum size of one side of the drag-select area before it will be considered valid. */
    static constexpr int minDragSelectSize = 8;

    void reset();
    bool isActive() const;
    bool isValid() const;

    // Mouse positions during drag-select, in window co-ordinates
    int startX = -1;
    int startY = -1;
    int endX = -1;
    int endY = -1;
};

/**
 * Data relating to the current selection and actions of the local player.
 */
struct PlayerContext
{
    DragSelect dragSelect;
    MapNode tileUnderMouse { -1, -1 };
    std::weak_ptr<Entity> weakEntityUnderMouse;
    WeakMutableEntityList weakSelectedEntities;

    std::set<int> getSelectedEntityIds() const;
};

}  // namespace Rival
