#include "Spritesheet.h"

#include "MathUtils.h"
#include "Texture.h"

namespace Rival {

Spritesheet::Spritesheet(std::shared_ptr<const Texture> texture, int width, int height, int padding)
    : texture(texture)
    , width(width)
    , height(height)
    , padding(padding)
    , paddedWidth(width + 2 * padding)
    , paddedHeight(height + 2 * padding)
    , xSize(texture->getWidth() / paddedWidth)
    , ySize(texture->getHeight() / paddedHeight)
{
}

const std::vector<GLfloat> Spritesheet::getTexCoords(int index) const
{
    const int x = index % xSize;
    const int y = index / xSize;
    const float txWidth = static_cast<float>(texture->getWidth());
    const float txHeight = static_cast<float>(texture->getHeight());

    const float srcX = static_cast<float>(x * paddedWidth + padding);
    const float srcY = static_cast<float>(y * paddedHeight + padding);

    const float tx1 = srcX / txWidth;
    const float tx2 = (srcX + width) / txWidth;
    const float ty1 = srcY / txHeight;
    const float ty2 = (srcY + height) / txHeight;

    return {
        tx1, ty1,  //
        tx2, ty1,  //
        tx2, ty2,  //
        tx1, ty2   //
    };
}

}  // namespace Rival
