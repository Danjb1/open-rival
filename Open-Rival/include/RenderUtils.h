#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <cmath>

namespace Rival {
namespace RenderUtils {

    // Size of a single Tile image, in pixels
    static const int tileSpriteWidthPx = 64;
    static const int tileSpriteHeightPx = 64;

    // Size of a single Tile image, ignoring empty space, in pixels
    static const int tileWidthPx = 64;
    static const int tileHeightPx = 32;

    // Size of a single Entity image, in pixels.
    // This seems to be broadly true for all Entities, but may need revisiting.
    static const int entityWidthPx = 128;
    static const int entityHeightPx = 128;

    // Offset applied to align an Entity's sprite correctly within a tile,
    // in pixels
    static const int entityDrawOffsetX = -33;
    static const int entityDrawOffsetY = -89;

    // Render limits.
    // These define the maximum number of tiles that can ever be rendered
    // at one time, and, by extension, the limits of our camera.
    // For now, this is set to the maximum map size in the original game.
    static const int maxTilesX = 210;
    static const int maxTilesY = 134;

    // Camera z-position.
    // We position our camera such that it looks into the screen.
    // All z co-ordinates should therefore be negative; lower values
    // represent greater depth.
    static const float zCamera = 1.0f;

    // Camera near plane.
    // This is the minimum distance from the camera that we can render.
    static const float nearPlane = 1.0f;

    // Z-positions used in rendering.
    // Lower values are nearer to the camera and will be rendered on top.
    static const float zMapBorder = zCamera - nearPlane;
    static const float zFog = zMapBorder - 1.0f;
    static const float zEntitiesEnd = zFog - 1.0f;
    static const float zEntitiesStart = zEntitiesEnd - maxTilesY;
    static const float zTiles = zEntitiesStart - 1.0f;

    // Camera far plane.
    // This is the maximum distance from the camera that we can render.
    // We calculate this such that we can handle all required depth values.
    static const float farPlane = nearPlane + abs(zTiles - 1.0f);

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

    /**
     * Converts from camera units to pixels, in the x-axis.
     */
    float cameraToPx_X(float x);

    /**
     * Converts from camera units to pixels, in the y-axis.
     */
    float cameraToPx_Y(float y);

    /**
     * Converts from pixels to camera units in the x-axis.
     */
    float pxToCamera_X(float x);

    /**
     * Converts from pixels to camera units in the y-axis.
     */
    float pxToCamera_Y(float y);

    /**
     * Gets the z-position of an entity at the given tile co-ordinates.
     */
    float getEntityZ(int x, int y);

    /**
     * Gets the width of the canvas required to render the area visible to
     * the given camera at a pixel-perfect scale.
     */
    int getCanvasWidth(float cameraWidth);

    /**
     * Gets the height of the canvas required to render the area visible to
     * the given camera at a pixel-perfect scale.
     */
    int getCanvasHeight(float cameraHeight);

}
}  // namespace Rival::RenderUtils

#endif  // RENDER_UTILS_H
