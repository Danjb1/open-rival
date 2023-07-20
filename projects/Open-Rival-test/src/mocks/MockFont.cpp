/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "gfx/Font.h"

namespace Rival {

Font Font::loadFont(
        FT_Library& /*ft*/, std::vector<std::string> /*fontDirs*/, std::string /*filename*/, int /*defaultSize*/)
{
    std::shared_ptr<const Texture> texture = {};
    std::unordered_map<char, CharData> chars = {};
    return { texture, chars, 0 };
}

Font::Font(std::shared_ptr<const Texture> texture, std::unordered_map<char, CharData> chars, int defaultSize)
    : texture(texture)
    , chars(chars)
    , defaultSize(defaultSize)
{
}

}  // namespace Rival
