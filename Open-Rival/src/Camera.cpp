#include "pch.h"
#include "Camera.h"

#include "MathUtils.h"

namespace Rival {

    Camera::Camera(
            float x,
            float y,
            float width,
            double aspectRatio,
            Scenario& scenario) :
        width(width),
        height(static_cast<float>(width / aspectRatio)),
        scenario(scenario) {

        setPos(x, y);
    }

    void Camera::setPos(float x, float y) {

        // Keep within the bounds of the map
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
        return width / zoom;
    }

    float Camera::getHeight() const {
        return height / zoom;
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

    bool Camera::contains(float px, float py) const {
        return px > getLeft()
                && px < getRight()
                && py > getTop()
                && py < getBottom();
    }

}
