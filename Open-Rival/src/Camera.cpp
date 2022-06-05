#include "pch.h"
#include "Camera.h"

#include <iostream>

#include "MapUtils.h"
#include "MathUtils.h"

namespace Rival {

    const float Camera::zoomInterval = 0.1f;
    const float Camera::zoomMin = 0.5f;
    const float Camera::zoomMax = 2.0f;

    const int Camera::tileWidth = 2;
    const int Camera::tileHeight = 1;

    const float Camera::bottomEdgePadding = tileHeight / 2.0f;

    Camera::Camera(float x, float y, float width, float height, Scenario& scenario)
        : defaultWidth(width)
        , defaultHeight(height)
        , scenario(scenario)
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
        int lastTileIndexX = scenario.getWidth() - 1;
        int lastTileIndexY = scenario.getHeight() - 1;
        int rightEdge = lastTileIndexX + tileWidth;
        float bottomEdge = lastTileIndexY + tileHeight + bottomEdgePadding;
        float maxX = rightEdge - (cameraWidth / 2);
        float maxY = bottomEdge - (cameraHeight / 2);

        x = MathUtils::clampf(newX, minX, maxX);
        y = MathUtils::clampf(newY, minY, maxY);
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
        centreOnPoint(x + dx, y + dy);
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
        return x - getWidth() / 2;
    }

    float Camera::getTop() const
    {
        return y - getHeight() / 2;
    }

    float Camera::getRight() const
    {
        return x + getWidth() / 2;
    }

    float Camera::getBottom() const
    {
        return y + getHeight() / 2;
    }

    void Camera::modZoom(float interval)
    {
        zoom += interval;
        zoom = MathUtils::clampf(zoom, zoomMin, zoomMax);

        // We call `centreOnPoint` here to perform a bounds check, since the
        // size of the visible region has now changed
        centreOnPoint(x, y);
    }

    bool Camera::contains(float px, float py) const
    {
        return px > getLeft() && px < getRight() && py > getTop() && py < getBottom();
    }

}  // namespace Rival
