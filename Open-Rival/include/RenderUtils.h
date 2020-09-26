#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <cmath>

namespace Rival {
namespace RenderUtils {

    // Size of a single Tile image, in pixels
    const int tileSpriteWidthPx = 64;
    const int tileSpriteHeightPx = 64;

    // Size of a single Tile image, ignoring empty space, in pixels
    const int tileWidthPx = 64;
    const int tileHeightPx = 32;

    // Size of a single Unit image, in pixels
    const int unitWidthPx = 128;
    const int unitHeightPx = 128;

    // Size of a single Building image, in pixels
    const int buildingWidthPx = 128;
    const int buildingHeightPx = 128;

    // Render limits.
    // These define the maximum number of tiles that can ever be rendered
    // at one time, and, by extension, the limits of our camera.
    const int maxTilesX = 50;
    const int maxTilesY = 100;

    // Camera z-position.
    // We position our camera such that it looks into the screen.
    // All z co-ordinates should therefore be negative; lower values
    // represent greater depth.
    const float zCamera = 1.0f;

    // Camera near plane.
    // This is the minimum distance from the camera that we can render.
    const float nearPlane = 1.0f;

    // Z-positions used in rendering.
    // Lower values are nearer to the camera and will be rendered on top.
    const float zMapBorder = zCamera - nearPlane;
    const float zFog = zMapBorder - 1.0f;
    const float zEntitiesEnd = zFog - 1.0f;
    const float zEntitiesStart = zEntitiesEnd - maxTilesY;
    const float zTiles = zEntitiesStart - 1.0f;

    // Camera far plane.
    // This is the maximum distance from the camera that we can render.
    // We calculate this such that we can handle all required depth values.
    const float farPlane = nearPlane + abs(zTiles - 1.0f);

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

    float getEntityZ(int x, int y);

}
}  // namespace Rival::RenderUtils

#endif  // RENDER_UTILS_H
