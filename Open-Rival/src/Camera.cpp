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
            Scenario& scenario) :
        defaultWidth(width),
        defaultHeight(static_cast<float>(width / aspectRatio)),
        scenario(scenario) {

        setPos(x, y);
    }

    void Camera::setPos(float x, float y) {

        // Keep within the bounds of the map
        float width = getWidth();
        float height = getHeight();
        float minX = width / 2;
        float minY = height / 2;
        float maxX = (scenario.getWidth() - 1) - (width / 2);
        float maxY = (scenario.getHeight() - 1) - (height / 2);
        x = MathUtils::clampf(x, minX, maxX);
        y = MathUtils::clampf(y, minY, maxY);

        this->x = x;
        this->y = y;
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
        // It is important that we return a whole number, otherwise it is
        // impossible to create a viewport of the exact same size, which can
        // result in rounding errors, and visible seams between tiles
        return ceil(defaultWidth / zoom);
    }

    float Camera::getHeight() const {
        // See notes in `getWidth()`
        return ceil(defaultHeight / zoom);
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

    void Camera::modZoom(float zoomInterval) {
        zoom += zoomInterval;
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

}
