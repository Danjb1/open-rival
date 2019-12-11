#include "pch.h"
#include "RenderUtils.h"

#include "Sprite.h"

namespace Rival {
    namespace RenderUtils {

        int getRenderPosX(int x) {
            // Tile co-ordinates are consistent in the x direction
            return x * (Sprite::tileWidthPx / 2);
        }

        int getRenderPosY(int x, int y) {
            // Tile co-ordinates zigzag up and down in the y direction
            int renderPos = y * Sprite::tileHeightPx;
            if (x % 2 == 1) {
                renderPos += Sprite::tileHeightPx / 2;
            }
            return renderPos;
        }

    }
}
