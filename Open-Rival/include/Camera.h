#ifndef CAMERA_H
#define CAMERA_H

#include "Scenario.h"

namespace Rival {

    class Camera {

    private:

        // The centre point of the camera
        float x;
        float y;

        float width;
        float height;

        Scenario& scenario;

    public:

        Camera(
            float x,
            float y,
            float width,
            double aspectRatio,
            Scenario& scenario);

        void setPos(float x, float y);

        float getX() const;

        float getY() const;

        float getWidth() const;

        float getHeight() const;

    };

}

#endif // CAMERA_H
