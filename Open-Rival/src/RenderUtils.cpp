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

    float getEntityZ(int x, int y) {
        // Depth is controlled by the y-position, so entities further down the
        // map will be rendered on top
        float z = RenderUtils::zEntitiesStart + static_cast<float>(y);

        // Tiles in a given row zigzag up and down.
        // Entities at the bottom of this zigzag should be rendered on top
        // of others in the same row.
        if (x % 2 == 1) {
            z += 0.5f;
        }

        return z;
    }

    int getCanvasWidth(float cameraWidth) {
        int canvasWidth = static_cast<int>(
                RenderUtils::worldToPx_X(cameraWidth));
        // Round up to the nearest even number.
        // This is crucial as this value gets divided by 2 when setting the
        // projection matrix using `glm::ortho`. For an odd number, floating
        // point errors could then cause a subtle difference between the
        // viewport size and the canvas size, which could ultimately lead to
        // seams between tiles.
        // For an explanation of this little bitwise trick, see:
        // https://stackoverflow.com/a/57106819/1624459
        return canvasWidth + (canvasWidth & 1);
    }

    int getCanvasHeight(float cameraHeight) {
        int canvasHeight = static_cast<int>(
                RenderUtils::worldToPx_Y(cameraHeight));
        // See comments in `getCanvasWidth`.
        return canvasHeight + (canvasHeight & 1);
    }

}
}  // namespace Rival::RenderUtils
