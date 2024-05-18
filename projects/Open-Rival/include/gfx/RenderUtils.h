#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <cmath>

#include "gfx/Camera.h"

namespace Rival { namespace RenderUtils {

// Size of a single Tile image, in pixels
static constexpr int tileSpriteWidthPx = 64;
static constexpr int tileSpriteHeightPx = 64;

// Size of a single Tile image, ignoring empty space, in pixels
static constexpr int tileWidthPx = 64;
static constexpr int tileHeightPx = 32;

// Size of a single Entity image, in pixels.
// This seems to be broadly true for all Entities, but may need revisiting.
static constexpr int entityWidthPx = 128;
static constexpr int entityHeightPx = 128;

// Offset applied to align an Entity's sprite correctly within a tile,
// in pixels.
static constexpr int entityDrawOffsetX = -33;
static constexpr int entityDrawOffsetY = -89;

// Size of the hitbox sprites, in pixels.
static constexpr int hitboxSpriteWidthPx = 16;
static constexpr int hitboxSpriteHeightPx = 16;

// Size of the cursor sprites, in pixels.
static constexpr int cursorWidthPx = 32;
static constexpr int cursorHeightPx = 32;

// Size of the camera used for menus.
// This defines the notion of "menu co-ordinates".
// This height value is based on the original menu image sizes.
// The width must be calculated at runtime based on the aspect ratio.
static constexpr int menuHeight = 600;

// Render limits.
// These define the maximum number of tiles that can ever be rendered
// at one time, and, by extension, the limits of our camera.
// For now, this is set to the maximum map size in the original game.
static constexpr int maxTilesX = 210;
static constexpr int maxTilesY = 134;

// Camera z-position.
// Our camera points along positive z.
static constexpr float cameraZ = -1.0f;

// Camera near plane.
// This is the minimum distance from the camera that we can render.
static constexpr float nearPlane = 1.0f;

// Z-positions used in rendering.
// Lower values are nearer to the camera and will be rendered on top.
static constexpr float zStart = cameraZ + nearPlane;
static constexpr float zMapBorder = zStart;
static constexpr float zOverlays = zMapBorder + 1;
static constexpr float zHitbox = zOverlays + 1;
static constexpr float zFog = zHitbox + 1.f;
static constexpr float zEntityNearest = zFog + 1.f;
static constexpr float zEntityFurthest = zEntityNearest + maxTilesY;
static constexpr float zTiles = zEntityFurthest + 1.f;

// Camera far plane.
// This is the maximum distance from the camera that we can render.
// We calculate this such that we can handle all required depth values.
static const float farPlane = nearPlane + abs(zTiles + 1.f);

// Camera up vector.
// Our game world uses a top-left origin, so +y is down.
static constexpr glm::vec3 upVector { 0.f, -1.f, 0.f };

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
