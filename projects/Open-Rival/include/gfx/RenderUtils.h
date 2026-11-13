#pragma once

#include <glm/mat4x4.hpp>

#include "gfx/RenderConstants.h"

namespace Rival {
class Camera;
}

namespace Rival { namespace RenderUtils {

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

/**
 * Gets the width of the camera used for menus.
 */
int getMenuWidth(const double aspectRatio);

/**
 * Creates a view matrix focused on the given point.
 */
glm::mat4 createViewMatrix(float cameraX, float cameraY);

/**
 * Creates a projection matrix using the given dimensions.
 */
glm::mat4 createProjectionMatrix(float width, float height);

/**
 * Creates a view-projection matrix for rendering to the game world.
 */
glm::mat4 createGameViewProjectionMatrix(const Camera& camera, int viewportWidth, int viewportHeight);

/**
 * Creates a view-projection matrix for rendering to a menu.
 */
glm::mat4 createMenuViewProjectionMatrix(double aspectRatio);

}}  // namespace Rival::RenderUtils
