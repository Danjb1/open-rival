#include "pch.h"
#include "Camera.h"

#include "MathUtils.h"

namespace Rival {

    const float Camera::zoomInterval = 0.1f;
    const float Camera::zoomMin = 0.5f;
    const float Camera::zoomMax = 2.0f;

    Camera::Camera(
            float x,
            float y,
            float width,
            double aspectRatio,
            Scenario& scenario)
        : defaultWidth(width),
          defaultHeight(static_cast<float>(width / aspectRatio)),
          scenario(scenario) {

        setPos(x, y);
    }

    void Camera::setPos(float newX, float newY) {

        // Keep within the bounds of the map
        float cameraWidth = getWidth();
        float cameraHeight = getHeight();
        float minX = cameraWidth / 2;
        float minY = cameraHeight / 2;

        // The last tile index is scenario.getWidth() - 1. However, we add 2
        // to account for the width of the last tile (each tile spans 2
        // columns due to the way they overlap).
        float rightEdge = static_cast<float>(scenario.getWidth() + 1);
        float maxX = rightEdge - (cameraWidth / 2);

        // Similarly, we add some value to the bottom edge to account for the
        // height of the last tile
        float bottomEdge = static_cast<float>(scenario.getHeight() + 0.5f);
        float maxY = bottomEdge - (cameraHeight / 2);

        x = MathUtils::clampf(newX, minX, maxX);
        y = MathUtils::clampf(newY, minY, maxY);
    }

    void Camera::translate(float dx, float dy) {
        setPos(x + dx, y + dy);
    }

    float Camera::getX() const {
        return x;
    }

    float Camera::getY() const {
        return y;
    }

    float Camera::getWidth() const {
        return defaultWidth / zoom;
    }

    float Camera::getHeight() const {
        return defaultHeight / zoom;
    }

    float Camera::getLeft() const {
        return x - getWidth() / 2;
    }

    float Camera::getTop() const {
        return y - getHeight() / 2;
    }

    float Camera::getRight() const {
        return x + getWidth() / 2;
    }

    float Camera::getBottom() const {
        return y + getHeight() / 2;
    }

    float Camera::getZoom() const {
        return zoom;
    }

    void Camera::modZoom(float interval) {
        zoom += interval;
        zoom = MathUtils::clampf(zoom, zoomMin, zoomMax);

        // We call `setPos` here to perform a bounds check, since the size of
        // the visible region has now changed
        setPos(x, y);
    }

    bool Camera::contains(float px, float py) const {
        return px > getLeft()
                && px < getRight()
                && py > getTop()
                && py < getBottom();
    }

}  // namespace Rival
