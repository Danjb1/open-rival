/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "gfx/Font.h"

namespace Rival {

Font::Font(std::shared_ptr<const Texture> texture, std::unordered_map<char, CharData> chars, int defaultSize)
    : texture(texture)
    , chars(chars)
    , defaultSize(defaultSize)
{
}

}  // namespace Rival
