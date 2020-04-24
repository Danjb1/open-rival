#ifndef CAMERA_H
#define CAMERA_H

#include "Scenario.h"

namespace Rival {

    class Camera {

    private:

        // The point at the centre of the camera, in world units
        float x;
        float y;

        // The size of the visible region, in world units.
        // This varies based on the zoom level.
        float width;
        float height;

        // Zoom level (render scale)
        float zoom = 1.0f;

        Scenario& scenario;

    public:

        Camera(
            float x,
            float y,
            float width,
            double aspectRatio,
            Scenario& scenario);

        void setPos(float x, float y);

        void translate(float dx, float dy);

        float getX() const;

        float getY() const;

        float getWidth() const;

        float getHeight() const;

        float getLeft() const;

        float getTop() const;

        float getRight() const;

        float getBottom() const;

        float getZoom() const;

        bool contains(float px, float py) const;

    };

}

#endif // CAMERA_H
