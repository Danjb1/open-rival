#include "pch.h"

#include "Cursor.h"

#include "PlayerContext.h"

namespace Rival {

CursorDef Cursor::getCurrentCursor(const PlayerContext& playerContext)
{
    if (auto entityUnderMouse = playerContext.weakEntityUnderMouse.lock())
    {
        return select;
    }

    // Default cursor
    return arrow;
}

}  // namespace Rival
