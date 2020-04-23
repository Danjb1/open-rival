#include "pch.h"
#include "RenderUtils.h"

#include "Spritesheet.h"

namespace Rival {
    namespace RenderUtils {

        int getRenderPosX(int x) {
            // Tile co-ordinates are consistent in the x direction
            return x * (tileSpriteWidthPx / 2);
        }

        int getRenderPosY(int x, int y) {
            // Tile co-ordinates zigzag up and down in the y direction
            int renderPos = (y * tileSpriteHeightPx / 2);
            if (x % 2 == 1) {
                renderPos += (tileSpriteHeightPx / 4);
            }
            return renderPos;
        }

    }
}
