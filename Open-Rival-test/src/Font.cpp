#include "pch.h"

#include "Font.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

namespace Rival {

Font::Font(Texture texture, std::unordered_map<char, CharData> chars, int defaultSize)
    : texture(texture)
    , chars(chars)
    , defaultSize(defaultSize)
{
}

}  // namespace Rival
