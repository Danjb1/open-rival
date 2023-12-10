#pragma once

#include "gfx/renderable/BoxRenderable.h"

namespace Rival {

struct PlayerContext;

/**
 * Class responsible for rendering the drag-select to the screen.
 */
class DragSelectRenderer
{
public:
    DragSelectRenderer(const PlayerContext& playerContext);

    void render();

private:
    static constexpr int numBoxes = 4;

    const PlayerContext& playerContext;

    BoxRenderable boxRenderable;
};

}  // namespace Rival
