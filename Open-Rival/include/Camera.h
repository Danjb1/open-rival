#ifndef CAMERA_H
#define CAMERA_H

#include "Scenario.h"

namespace Rival {

    class Camera {

    public:
        static const float zoomInterval;
        static const float zoomMin;
        static const float zoomMax;

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

        void modZoom(float zoomInterval);

        bool contains(float px, float py) const;

    private:
        // The point at the centre of the camera, in world units
        float x;
        float y;

        // The size of the visible region, in world units, at the default zoom
        // level.
        // The public-facing getters always take into account the zoom level.
        float defaultWidth;
        float defaultHeight;

        // Zoom level (render scale)
        float zoom = 1.0f;

        Scenario& scenario;
    };

}  // namespace Rival

#endif  // CAMERA_H
