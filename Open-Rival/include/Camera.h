#ifndef CAMERA_H
#define CAMERA_H

#include "Scenario.h"

namespace Rival {

    /**
     * The camera uses its own co-ordinate system ("camera units"):
     *
     *     1 camera unit = 32 scaled px
     *
     * where 1 scaled px is equivalent to the size of a rendered pixel.
     *
     * This is necessary because the game world is not a continuous space; the
     * tiles zigzag up and down, so the conversion between tile co-ordinates
     * and pixel positioning is not straightforward. If we tried to position
     * the camera in tile space, it would bob up and down while panning left
     * and right!
     */
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

        void centreOnPoint(float x, float y);

        void centreOnTile(int x, int y);

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
        // Size of a tile, in camera co-ordinates
        static const float tileWidthCamera;
        static const float tileHeightCamera;

        // Extra distance the camera is allowed to travel at the bottom of the
        // map. This is necessary because a row of tiles actually spans more
        // than 1 tile height due to their zigzagging y-positioning.
        static const float bottomEdgePadding;

        // The point at the centre of the camera, in camera units
        float x;
        float y;

        // The size of the visible region, in camera units, at the default zoom
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
