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
        height(static_cast<float>(aspectRatio / width)),
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

}
