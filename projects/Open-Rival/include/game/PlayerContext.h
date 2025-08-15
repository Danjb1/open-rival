#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <vector>

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

/** A non-instantaneous action that can be performed by a local player. */
enum class PlayerAction : std::uint8_t
{
    None,
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

    void setCurrentAction(PlayerAction newMode);
    PlayerAction getCurrentAction() const
    {
        return currentAction;
    }

    std::set<int> getSelectedEntityIds() const;
    std::vector<int> getSelectedEntityIdsVector() const;
    std::weak_ptr<Entity> getFirstSelectedEntity() const;
    void deselectEntity(int entityId);

private:
    PlayerAction currentAction = PlayerAction::None;
};

}  // namespace Rival
