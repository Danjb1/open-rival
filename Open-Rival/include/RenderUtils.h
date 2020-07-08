#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

namespace Rival {
namespace RenderUtils {

    // Size of a single Tile image, in pixels
    static const int tileSpriteWidthPx = 64;
    static const int tileSpriteHeightPx = 64;

    // Size of a single Tile image, ignoring empty space, in pixels
    static const int tileWidthPx = 64;
    static const int tileHeightPx = 32;

    // Size of a single Unit image, in pixels
    static const int unitWidthPx = 128;
    static const int unitHeightPx = 128;

    // Size of a single Building image, in pixels
    static const int buildingWidthPx = 128;
    static const int buildingHeightPx = 128;

    // Render limits.
    // These define the maximum number of tiles that can ever be rendered
    // at one time, and, by extension, the limits of our camera.
    static const int maxTilesX = 50;
    static const int maxTilesY = 100;

    /**
     * Gets the x-position at which a tile with the given x co-ordinate
     * should be rendered, in pixels.
     *
     * Normally we would send our vertices to the GPU in world units, and
     * rely on the OpenGL projection to handle the conversion to pixels.
     * However, there is no way OpenGL can position our tiles for us due
     * to their strange isometric positioning, so we convert our vertices
     * to pixels manually instead.
     */
    int tileToPx_X(int x);

    /**
     * Gets the y-position at which a tile with the given x and y
     * co-ordinates should be rendered, in pixels.
     *
     * We have to consider both axes here because the y-position is
     * actually dependent on the x-position.
     *
     * See documentation for tileToPx_X, above.
     */
    int tileToPx_Y(int x, int y);

    /**
     * Gets the x-position of a rendered tile, taking into account the
     * given zoom level.
     *
     * This is not useful during rendering since the zoom is only applied
     * AFTER rendering to the framebuffer, but it is useful for mouse
     * picking when we need to know the rendered position of an object.
     */
    float tileToScaledPx_X(int x, float zoomLevel);

    /**
     * Gets the y-position of a rendered tile, taking into account the
     * given zoom level.
     *
     * See documentation for tileToScaledPx_X, above.
     */
    float tileToScaledPx_Y(int x, int y, float zoomLevel);

    float worldToPx_X(float x);

    float worldToPx_Y(float y);

    float pxToWorld_X(float x);

    float pxToWorld_Y(float y);

}
}  // namespace Rival::RenderUtils

#endif  // RENDER_UTILS_H
