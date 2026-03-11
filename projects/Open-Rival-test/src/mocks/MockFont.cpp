#include "gfx/Font.h"

namespace Rival {

std::shared_ptr<const Font> Font::loadFont(FT_Library, std::string, int, Renderer*)
{
    std::shared_ptr<const Texture> texture = std::make_shared<const Texture>(0, 128, 128);
    std::unordered_map<char, CharData> chars = {};
    int defaultSize = 10;
    return std::make_shared<const Font>(texture, chars, defaultSize);
}

std::shared_ptr<const Font> Font::loadFont(
        FT_Library, std::vector<std::string>, std::string, int defaultSize, Renderer*)
{
    std::shared_ptr<const Texture> texture = std::make_shared<const Texture>(0, 128, 128);
    std::unordered_map<char, CharData> chars = {};
    return std::make_shared<const Font>(texture, chars, defaultSize);
}

Font::Font(std::shared_ptr<const Texture> texture, std::unordered_map<char, CharData> chars, int defaultSize)
    : texture(texture)
    , chars(chars)
    , defaultSize(defaultSize)
{
}

}  // namespace Rival
