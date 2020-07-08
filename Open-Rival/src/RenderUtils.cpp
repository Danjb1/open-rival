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

    float tileToScaledPx_X(int x, float zoomLevel) {
        // Same as tileToPx_X, above, but with a zoom level applied
        float scaledTileWidth = tileSpriteWidthPx * zoomLevel;
        return x * (scaledTileWidth / 2);
    }

    float tileToScaledPx_Y(int x, int y, float zoomLevel) {
        // Same as tileToPx_Y, above, but with a zoom level applied
        float scaledTileHeight = tileSpriteHeightPx * zoomLevel;
        float renderPos = y * (scaledTileHeight / 2);
        if (x % 2 == 1) {
            renderPos += (scaledTileHeight / 4);
        }
        return renderPos;
    }

    float worldToPx_X(float x) {
        // Same as tileToPx_X, above, but using floats.
        // This is especially important for the camera, since it can be
        // "between" tiles.
        return x * (tileSpriteWidthPx / 2);
    }

    float worldToPx_Y(float y) {
        // Same as tileToPx_Y, above, but using floats, and without the
        // zigzagging.
        // This is especially important for the camera, since we don't
        // want it to move up and down when scrolling left and right!
        // More generally, this should be used to calculate distances
        // rather than actual positions.
        return y * (tileSpriteHeightPx / 2);
    }

    float pxToWorld_X(float x) {
        // This is the reverse of worldToPx_X
        return 2 * (x / tileSpriteWidthPx);
    }

    float pxToWorld_Y(float y) {
        // This is the reverse of worldToPx_Y
        return 2 * (y / tileSpriteHeightPx);
    }

}
}  // namespace Rival::RenderUtils
