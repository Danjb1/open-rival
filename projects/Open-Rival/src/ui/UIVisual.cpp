#include "ui/UIVisual.h"

#include <cmath>  // std::ceil

#include "gfx/Font.h"
#include "gfx/TextureAtlas.h"

namespace Rival {

glm::ivec2 BoxVisual::getNaturalSize() const
{
    return { 0, 0 };
}

glm::ivec2 ImageVisual::getNaturalSize() const
{
    return { atlas->getImageWidth(imageKey), atlas->getImageHeight(imageKey) };
}

glm::ivec2 TextVisual::getNaturalSize() const
{
    const Font& font = *properties.font;
    const int width = static_cast<int>(std::ceil(font.getTextWidth(span.text, properties.scale)));
    const int height = static_cast<int>(std::ceil(font.getLineHeight(properties.scale)));
    return { width, height };
}

glm::ivec2 getNaturalSize(const UIVisual& visual)
{
    return std::visit([](const auto& v) { return v.getNaturalSize(); }, visual);
}

}  // namespace Rival
