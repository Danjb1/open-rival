#include "utils/Rect.h"

namespace Rival {

Rect::Rect(int x, int y, int width, int height)
    : x(static_cast<float>(x))
    , y(static_cast<float>(y))
    , width(static_cast<float>(width))
    , height(static_cast<float>(height))
{
}

Rect::Rect(float x, float y, float width, float height)
    : x(x)
    , y(y)
    , width(width)
    , height(height)
{
}

bool Rect::contains(int px, int py) const
{
    return px >= x             //
            && px < x + width  //
            && py >= y         //
            && py < y + height;
}

bool Rect::contains(float px, float py) const
{
    return px >= x             //
            && px < x + width  //
            && py >= y         //
            && py < y + height;
}

bool Rect::intersects(Rect other) const
{
    // See: https://medium.com/@jessgillan/algorithm-practice-rectangle-intersection-7821411fd114
    return x <= (other.x + other.width)       //
            && (x + width) >= other.x         //
            && y <= (other.y + other.height)  //
            && (y + height) >= other.y;
}

}  // namespace Rival
