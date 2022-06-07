#include "pch.h"

#include "Rect.h"

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
        return px >= x && px < x + width && py >= y && py < y + height;
    }

    bool Rect::contains(float px, float py) const
    {
        return px >= x && px < x + width && py >= y && py < y + height;
    }

}  // namespace Rival
