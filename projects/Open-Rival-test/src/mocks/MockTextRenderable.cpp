/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "gfx/renderable/TextRenderable.h"

namespace Rival {

const Color TextRenderable::defaultColor = Color::makeRgb(255, 221, 65);
const Color TextRenderable::highlightColor = Color::makeRgb(255, 43, 40);

TextRenderable::TextRenderable(TextSpan /*span*/, TextProperties /*props*/, float /*x*/, float /*y*/) {}

TextRenderable::TextRenderable(
        std::vector<TextSpan> /*spans*/, const TextProperties /*props*/, float /*x*/, float /*y*/)
{
}

TextRenderable::TextRenderable(int /*maxChars*/, TextProperties /*props*/, float /*x*/, float /*y*/) {}

TextRenderable::TextRenderable(TextRenderable&& /*other*/) noexcept {}

TextRenderable::~TextRenderable() {}

}  // namespace Rival
