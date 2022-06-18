#pragma once

namespace Rival {

class World;

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
class Camera
{

public:
    // Size of a tile, in camera co-ordinates
    static constexpr int tileWidth = 2;
    static constexpr int tileHeight = 1;

    // Zoom behaviour
    static constexpr float zoomInterval = 0.1f;
    static constexpr float zoomMin = 0.5f;
    static constexpr float zoomMax = 2.0f;

    Camera(float x, float y, float width, float height, World& world);

    void centreOnPoint(float x, float y);

    void centreOnTile(int x, int y);

    /**
     * Moves the camera by some distance, in camera units.
     *
     * Translating the camera by (1, 1) is equivalent to moving it by 1 row
     * and 1 column; each tile actually measures 2 camera units, but the
     * columns overlap.
     */
    void translate(float dx, float dy);

    float getX() const
    {
        return x;
    }

    float getY() const
    {
        return y;
    }

    float getWidth() const;

    float getHeight() const;

    float getLeft() const;

    float getTop() const;

    float getRight() const;

    float getBottom() const;

    float getZoom() const
    {
        return zoom;
    }

    void modZoom(float zoomInterval);

    bool contains(float px, float py) const;

private:
    // Extra distance the camera is allowed to travel at the bottom of the
    // map. This is necessary because a row of tiles actually spans more
    // than 1 tile height due to their zigzagging y-positioning.
    static constexpr float bottomEdgePadding = tileHeight / 2.0f;

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

    World& world;
};

}  // namespace Rival
