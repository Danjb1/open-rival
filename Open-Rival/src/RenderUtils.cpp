#include "pch.h"
#include "RenderUtils.h"

#include <glm/vec3.hpp>
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

#include "MapUtils.h"
#include "Spritesheet.h"

namespace Rival { namespace RenderUtils {

    int tileToPx_X(int x)
    {
        // Tile co-ordinates are consistent in the x direction.
        // We divide by 2 because the tiles overlap; 10 tiles in a row
        // actually have the width of 5.
        return x * (tileWidthPx / 2);
    }

    int tileToPx_Y(int x, int y)
    {
        // Rows are positioned directly below each other
        int renderPos = y * tileHeightPx;

        if (MapUtils::isLowerTile(x))
        {
            // Tile co-ordinates zigzag up and down within a row
            renderPos += (tileHeightPx / 2);
        }

        return renderPos;
    }

    float tileToScaledPx_X(int x, float zoomLevel)
    {
        // Same as tileToPx_X, above, but with a zoom level applied
        float scaledTileWidth = tileWidthPx * zoomLevel;
        return x * (scaledTileWidth / 2);
    }

    float tileToScaledPx_Y(int x, int y, float zoomLevel)
    {
        // Same as tileToPx_Y, above, but with a zoom level applied
        float scaledTileHeight = tileHeightPx * zoomLevel;
        float renderPos = y * scaledTileHeight;
        if (MapUtils::isLowerTile(x))
        {
            renderPos += (scaledTileHeight / 2);
        }
        return renderPos;
    }

    float cameraToPx_X(float x)
    {
        // Same as tileToPx_X, above, but using floats, since the camera can be
        // "between" tiles.
        return x * (tileWidthPx / 2);
    }

    float cameraToPx_Y(float y)
    {
        // Same as tileToPx_Y, above, but using floats, since the camera can be
        // "between" tiles. This also the omits the zigzagging aspect, since we
        // don't want the camera to move up and down when panning left and
        // right!
        return y * tileHeightPx;
    }

    float pxToCamera_X(float x)
    {
        // This is the reverse of cameraToPx_X
        return 2 * (x / tileWidthPx);
    }

    float pxToCamera_Y(float y)
    {
        // This is the reverse of cameraToPx_Y
        return y / tileHeightPx;
    }

    float getEntityZ(int x, int y)
    {
        // Depth is controlled by the y-position, so entities further down the
        // map will be rendered on top
        float z = RenderUtils::zEntitiesStart + static_cast<float>(y);

        // Tiles in a given row zigzag up and down.
        // Entities at the bottom of this zigzag should be rendered on top
        // of others in the same row.
        if (MapUtils::isLowerTile(x))
        {
            z += 0.5f;
        }

        return z;
    }

    int getCanvasWidth(float cameraWidth)
    {
        int canvasWidth = static_cast<int>(RenderUtils::cameraToPx_X(cameraWidth));
        // Round up to the nearest even number.
        // This is crucial as this value gets divided by 2 when setting the
        // projection matrix using `glm::ortho`. For an odd number, floating
        // point errors could then cause a subtle difference between the
        // viewport size and the canvas size, which could ultimately lead to
        // seams between tiles.
        // For an explanation of this little bitwise trick, see:
        // https://stackoverflow.com/a/57106819/1624459
        return canvasWidth + (canvasWidth & 1);
    }

    int getCanvasHeight(float cameraHeight)
    {
        int canvasHeight = static_cast<int>(RenderUtils::cameraToPx_Y(cameraHeight));
        // See comments in `getCanvasWidth`.
        return canvasHeight + (canvasHeight & 1);
    }

    int getMenuWidth(double aspectRatio)
    {
        return static_cast<int>(menuHeight * aspectRatio);
    }

    glm::mat4 createGameProjection(const Camera& camera, int viewportWidth, int viewportHeight)
    {

        // Determine view matrix.
        //
        // OpenGL uses right-handed rule:
        //  - x points right
        //  - y points up
        //  - z points out of the screen
        //
        // The camera co-ordinates are in camera units, but our vertices are
        // positioned using pixels. Therefore we need to convert the camera
        // co-ordinates to pixels, too.
        float cameraX = RenderUtils::cameraToPx_X(camera.getX());
        float cameraY = RenderUtils::cameraToPx_Y(camera.getY());
        float cameraZ = RenderUtils::zCamera;
        glm::mat4 view = glm::lookAt(
                glm::vec3(cameraX, cameraY, cameraZ),  // camera position
                glm::vec3(cameraX, cameraY, 0),        // look at
                glm::vec3(0, 1, 0)                     // up vector
        );

        // Determine projection matrix.
        // The projection size must match the viewport size *exactly* in order
        // to achieve pixel-perfect rendering. Any differences here could
        // result in seams between tiles.
        float left = -viewportWidth / 2.0f;
        float top = -viewportHeight / 2.0f;
        float right = viewportWidth / 2.0f;
        float bottom = viewportHeight / 2.0f;
        float near = RenderUtils::nearPlane;
        float far = RenderUtils::farPlane;
        glm::mat4 projection = glm::ortho(left, right, bottom, top, near, far);

        // Combine matrices
        return projection * view;
    }

    glm::mat4 createMenuProjection(double aspectRatio)
    {

        // Determine view matrix
        // (camera looking at the centre of the menu)
        int menuWidth = RenderUtils::getMenuWidth(aspectRatio);
        float cameraX = menuWidth / 2.0f;
        float cameraY = menuHeight / 2.0f;
        float cameraZ = RenderUtils::zCamera;
        glm::mat4 view = glm::lookAt(
                glm::vec3(cameraX, cameraY, cameraZ),  // camera position
                glm::vec3(cameraX, cameraY, 0),        // look at
                glm::vec3(0, 1, 0)                     // up vector
        );

        // Determine projection matrix
        float left = -menuWidth / 2.0f;
        float top = -menuHeight / 2.0f;
        float right = menuWidth / 2.0f;
        float bottom = menuHeight / 2.0f;
        float near = RenderUtils::nearPlane;
        float far = RenderUtils::farPlane;
        glm::mat4 projection = glm::ortho(left, right, bottom, top, near, far);

        // Combine matrices
        return projection * view;
    }

}}  // namespace Rival::RenderUtils
