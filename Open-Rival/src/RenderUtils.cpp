#include "pch.h"
#include "RenderUtils.h"

#include "Spritesheet.h"

namespace Rival {
    namespace RenderUtils {

        int getRenderPosX(int x) {
            // Tile co-ordinates are consistent in the x direction
            return x * (Spritesheet::tileWidthPx / 2);
        }

        int getRenderPosY(int x, int y) {
            // Tile co-ordinates zigzag up and down in the y direction
            int renderPos = y * Spritesheet::tileHeightPx;
            if (x % 2 == 1) {
                renderPos += Spritesheet::tileHeightPx / 2;
            }
            return renderPos;
        }

    }
}
