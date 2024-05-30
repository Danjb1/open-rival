#pragma once

#include <cstdint>
#include <memory>
#include <set>

#include "entity/Entity.h"
#include "game/MapUtils.h"
#include "utils/EntityUtils.h"

namespace Rival {

struct DragSelect
{
    /** Minimum size of one side of the drag-select area before it will be considered valid. */
    static constexpr int minDragSelectSize = 5;

    void reset();
    bool isActive() const;
    bool isValid() const;

    // Mouse positions during drag-select, in window co-ordinates
    int startX = -1;
    int startY = -1;
    int endX = -1;
    int endY = -1;
};

/** A mode describing an action in progress. */
enum class ActionMode : std::uint8_t
{
    Default,
    DragSelect,
    Attack
};

/** Data relating to the current selection and actions of the local player. */
struct PlayerContext
{
    DragSelect dragSelect;
    MapNode tileUnderMouse { -1, -1 };
    std::weak_ptr<Entity> weakEntityUnderMouse;
    WeakMutableEntityList weakSelectedEntities;

    void setCurrentMode(ActionMode newMode);
    ActionMode getCurrentMode() const
    {
        return currentMode;
    }

    std::set<int> getSelectedEntityIds() const;

private:
    ActionMode currentMode = ActionMode::Default;
};

}  // namespace Rival
