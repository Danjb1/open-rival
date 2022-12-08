#include "pch.h"

#include "Spritesheet.h"

#include "MathUtils.h"
#include "Texture.h"

namespace Rival {

Spritesheet::Spritesheet(const Texture& texture, int width, int height)
    : texture(texture)
    , width(width)
    , height(height)
    , paddedWidth(MathUtils::nextPowerOf2(width))
    , paddedHeight(MathUtils::nextPowerOf2(height))
    , xSize(texture.getWidth() / paddedWidth)
    , ySize(texture.getHeight() / paddedHeight)
{
}

const std::vector<GLfloat> Spritesheet::getTexCoords(int index) const
{
    const int x = index % xSize;
    const int y = index / xSize;
    const float txWidth = static_cast<float>(texture.getWidth());
    const float txHeight = static_cast<float>(texture.getHeight());

    const float tx1 = x * paddedWidth / txWidth;
    const float tx2 = (x * paddedWidth + width) / txWidth;
    const float ty1 = y * paddedHeight / txHeight;
    const float ty2 = (y * paddedHeight + height) / txHeight;

    return {
        tx1, ty1,  //
        tx2, ty1,  //
        tx2, ty2,  //
        tx1, ty2   //
    };
}

}  // namespace Rival
