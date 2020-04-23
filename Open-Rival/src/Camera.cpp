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

        // Keep within bounds
        x = clampf(x, width / 2, scenario.getWidth() - width / 2);
        y = clampf(y, height / 2, scenario.getHeight() - height / 2);

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
        return width;
    }

    float Camera::getHeight() const {
        return height;
    }

    float Camera::getLeft() const {
        return x - width / 2;
    }

    float Camera::getTop() const {
        return y - height / 2;
    }

    float Camera::getRight() const {
        return x + width / 2;
    }

    float Camera::getBottom() const {
        return y + height / 2;
    }

}
