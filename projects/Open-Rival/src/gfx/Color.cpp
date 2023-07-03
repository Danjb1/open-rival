#include "gfx/Color.h"

#include <cstdint>

namespace Rival {

Color Color::makeRgb(int r, int g, int b)
{
    return Color(
            static_cast<std::uint8_t>(r) / 255.0f,
            static_cast<std::uint8_t>(g) / 255.0f,
            static_cast<std::uint8_t>(b) / 255.0f);
}

Color::Color(float r, float g, float b)
    : r(r)
    , g(g)
    , b(b)
    , a(1.0f)
{
}

}  // namespace Rival
