#include "gfx/Camera.h"

#include <iostream>

#include "game/MapUtils.h"
#include "game/World.h"
#include "utils/MathUtils.h"

namespace Rival {

Camera::Camera(float x, float y, float width, float height, World& world)
    : defaultWidth(width)
    , defaultHeight(height)
    , world(world)
{
    centreOnPoint(x, y);
}

void Camera::centreOnPoint(float newX, float newY)
{
    // Keep within the bounds of the map
    float cameraWidth = getWidth();
    float cameraHeight = getHeight();
    float minX = cameraWidth / 2;
    float minY = cameraHeight / 2;

    // Find the furthest point visible to the camera
    int lastTileIndexX = world.getWidth() - 1;
    int lastTileIndexY = world.getHeight() - 1;
    int rightEdge = lastTileIndexX + tileWidth;
    float bottomEdge = lastTileIndexY + tileHeight + bottomEdgePadding;
    float maxX = rightEdge - (cameraWidth / 2);
    float maxY = bottomEdge - (cameraHeight / 2);

    position.x = MathUtils::clampf(newX, minX, maxX);
    position.y = MathUtils::clampf(newY, minY, maxY);
}

void Camera::centreOnTile(int tileX, int tileY)
{
    float offsetY = 0;
    if (MapUtils::isLowerTile(tileX))
    {
        // Tile co-ordinates zigzag up and down within a row
        offsetY = (tileHeight / 2.0f);
    }

    centreOnPoint(tileX + (tileWidth / 2.0f), tileY + (tileHeight / 2.0f) + offsetY);
}

void Camera::translate(float dx, float dy)
{
    centreOnPoint(position.x + dx, position.y + dy);
}

float Camera::getWidth() const
{
    return defaultWidth / zoom;
}

float Camera::getHeight() const
{
    return defaultHeight / zoom;
}

float Camera::getLeft() const
{
    return position.x - getWidth() / 2;
}

float Camera::getTop() const
{
    return position.y - getHeight() / 2;
}

float Camera::getRight() const
{
    return position.x + getWidth() / 2;
}

float Camera::getBottom() const
{
    return position.y + getHeight() / 2;
}

void Camera::modZoom(float interval)
{
    zoom = MathUtils::clampf(zoom + interval, zoomMin, zoomMax);

    // We call `centreOnPoint` here to perform a bounds check, since the
    // size of the visible region has now changed
    centreOnPoint(position.x, position.y);
}

bool Camera::contains(float px, float py) const
{
    return px > getLeft() && px < getRight() && py > getTop() && py < getBottom();
}

}  // namespace Rival
