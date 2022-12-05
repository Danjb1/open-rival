#include "pch.h"

#include "RenderUtils.h"

#include <glm/vec3.hpp>
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

#include "Camera.h"
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

float pxToScaledPx_X(int x, float zoomLevel)
{
    // Same as tileToPx_X, above, but with a zoom level applied
    float scaledTileWidth = tileWidthPx * zoomLevel;
    return x * (scaledTileWidth / 2);
}

float cameraToPx_X(float x)
{
    // Same as tileToPx_X, above, but using floats, since the camera can be
    // "between" tiles.
    return x * static_cast<float>(tileWidthPx / 2);
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
    float z = zEntityFurthest - static_cast<float>(y);

    // Tiles in a given row zigzag up and down.
    // Entities at the bottom of this zigzag should be rendered on top
    // of others in the same row.
    if (MapUtils::isLowerTile(x))
    {
        z -= 0.5f;
    }

    return z;
}

int getCanvasWidth(float cameraWidth)
{
    int canvasWidth = static_cast<int>(cameraToPx_X(cameraWidth));
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
    int canvasHeight = static_cast<int>(cameraToPx_Y(cameraHeight));
    // See comments in `getCanvasWidth`.
    return canvasHeight + (canvasHeight & 1);
}

int getMenuWidth(double aspectRatio)
{
    return static_cast<int>(menuHeight * aspectRatio);
}

glm::mat4 createViewMatrix(float cameraX, float cameraY)
{
    // This assumes our vertices are positioned using this co-ordinate system:
    //  - +x points right
    //  - +y points down
    //  - +z points out of the screen
    return glm::lookAt(
            glm::vec3(cameraX, cameraY, cameraZ),  // camera position
            glm::vec3(cameraX, cameraY, 0.f),      // look at
            upVector                               // camera up
    );
}

glm::mat4 createProjectionMatrix(float width, float height)
{
    float left = -width / 2.0f;
    float top = height / 2.0f;
    float right = width / 2.0f;
    float bottom = -height / 2.0f;
    float near = nearPlane;
    float far = farPlane;
    return glm::ortho(left, right, bottom, top, near, far);
}

glm::mat4 createGameViewProjectionMatrix(const Camera& camera, int viewportWidth, int viewportHeight)
{
    // The camera co-ordinates are in camera units, but our vertices are
    // positioned using pixels. Therefore we need to convert the camera
    // co-ordinates to pixels, too.
    const glm::vec2& cameraPos = camera.getPosition();
    float cameraX = cameraToPx_X(cameraPos.x);
    float cameraY = cameraToPx_Y(cameraPos.y);
    glm::mat4 view = createViewMatrix(cameraX, cameraY);

    // The projection size must match the viewport size *exactly* in order
    // to achieve pixel-perfect rendering. Any differences here could
    // result in seams between tiles.
    glm::mat4 projection =
            createProjectionMatrix(static_cast<float>(viewportWidth), static_cast<float>(viewportHeight));

    // Combine matrices
    return projection * view;
}

glm::mat4 createMenuViewProjectionMatrix(double aspectRatio)
{
    // We position our camera to look at the centre of the menu
    int menuWidth = getMenuWidth(aspectRatio);
    float cameraX = menuWidth / 2.0f;
    float cameraY = menuHeight / 2.0f;
    glm::mat4 view = createViewMatrix(cameraX, cameraY);

    // Determine projection matrix
    glm::mat4 projection = createProjectionMatrix(static_cast<float>(menuWidth), static_cast<float>(menuHeight));

    // Combine matrices
    return projection * view;
}

}}  // namespace Rival::RenderUtils
