#include "pch.h"
#include "RenderUtils.h"

#include "Spritesheet.h"

namespace Rival {
    namespace RenderUtils {

        int tileToPx_X(int x) {
            // Tile co-ordinates are consistent in the x direction.
            // We divide by 2 because the tiles overlap; 10 tiles in a row
            // actually have the width of 5.
            return x * (tileSpriteWidthPx / 2);
        }

        int tileToPx_Y(int x, int y) {
            // We divide by 2 because the tiles overlap; 10 tiles in a column
            // actually have the height of 5.
            int renderPos = y * (tileSpriteHeightPx / 2);

            if (x % 2 == 1) {
                // Tile co-ordinates zigzag up and down in the y direction
                renderPos += (tileSpriteHeightPx / 4);
            }

            return renderPos;
        }

        float cameraToPx_X(float x) {
            // Same as tileToPx_X, above, but using floats, since the camera
            // can be "between" tiles
            return x * (tileSpriteWidthPx / 2);
        }

        float cameraToPx_Y(float y) {
            // Same as tileToPx_Y, above, but without the zigzagging; we
            // don't want the camera to move up and down when scrolling left
            // and right!
            return y * (tileSpriteHeightPx / 2);
        }

        float pxToCamera_X(float x) {
            // This is the reverse of cameraToPx_X
            return 2 * (x / tileSpriteWidthPx);
        }

        float pxToCamera_Y(float y) {
            // This is the reverse of cameraToPx_Y
            return 2 * (y / tileSpriteHeightPx);
        }

    }
}
