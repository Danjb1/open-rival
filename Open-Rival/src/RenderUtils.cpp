#include "pch.h"
#include "RenderUtils.h"

#include "Spritesheet.h"

namespace Rival {
namespace RenderUtils {

    int tileToPx_X(int x) {
        // Tile co-ordinates are consistent in the x direction.
        // We divide by 2 because the tiles overlap; 10 tiles in a row
        // actually have the width of 5.
        return x * (tileWidthPx / 2);
    }

    int tileToPx_Y(int x, int y) {
        // Rows are positioned directly below each other
        int renderPos = y * tileHeightPx;

        if (x % 2 == 1) {
            // Tile co-ordinates zigzag up and down within a row
            renderPos += (tileHeightPx / 2);
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

    float cameraToPx_X(float x) {
        // Same as tileToPx_X, above, but using floats, since the camera can be
        // "between" tiles.
        return x * (tileWidthPx / 2);
    }

    float cameraToPx_Y(float y) {
        // Same as tileToPx_Y, above, but using floats, since the camera can be
        // "between" tiles. This also the omits the zigzagging aspect, since we
        // don't want the camera to move up and down when panning left and
        // right!
        return y * tileHeightPx;
    }

    float pxToCamera_X(float x) {
        // This is the reverse of cameraToPx_X
        return 2 * (x / tileWidthPx);
    }

    float pxToCamera_Y(float y) {
        // This is the reverse of cameraToPx_Y
        return y / tileHeightPx;
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
                RenderUtils::cameraToPx_X(cameraWidth));
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
                RenderUtils::cameraToPx_Y(cameraHeight));
        // See comments in `getCanvasWidth`.
        return canvasHeight + (canvasHeight & 1);
    }

}
}  // namespace Rival::RenderUtils
