#pragma once

/*
 * Constants used in rendering.
 *
 * These are kept separate from RenderUtils.h so that headers which only need
 * these values do not need to depend on the rendering functions.
 */

#include <glm/vec3.hpp>

#include <cmath>

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
static constexpr int entityDrawOffsetX = -34;
static constexpr int entityDrawOffsetY = -89;

// Size of the hitbox sprites, in pixels.
static constexpr int hitboxSpriteWidthPx = 16;
static constexpr int hitboxSpriteHeightPx = 16;

// Size of the non-empty portion of the hitbox sprites, in pixels.
static constexpr int hitboxSpriteUsefulWidthPx = 10;
static constexpr int hitboxSpriteUsefulHeightPx = 10;

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
static constexpr float zEffectsNearest = zFog + 1.f;
static constexpr float zEffectsFurthest = zEffectsNearest + maxTilesY;
static constexpr float zProjectileNearest = zEffectsFurthest + 1.f;
static constexpr float zProjectileFurthest = zProjectileNearest + maxTilesY;
static constexpr float zFlyerNearest = zProjectileFurthest + 1.f;
static constexpr float zFlyerFurthest = zFlyerNearest + maxTilesY;
static constexpr float zEntityNearest = zFlyerFurthest + 1.f;
static constexpr float zEntityFurthest = zEntityNearest + maxTilesY;
static constexpr float zTiles = zEntityFurthest + 1.f;

// Z-position offset applied to projectiles
static constexpr float zOffsetProjectiles = zProjectileNearest - zEntityNearest;

// Z-position offset applied to flying units
static constexpr float zOffsetFlyers = zFlyerNearest - zEntityNearest;

// Z-position offset applied to effects
static constexpr float zOffsetEffects = zEffectsNearest - zEntityNearest;

// Camera far plane.
// This is the maximum distance from the camera that we can render.
// We calculate this such that we can handle all required depth values.
static const float farPlane = nearPlane + abs(zTiles + 1.f);

// Camera up vector.
// Our game world uses a top-left origin, so +y is down.
static constexpr glm::vec3 upVector { 0.f, -1.f, 0.f };

}}  // namespace Rival::RenderUtils
