/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "gfx/renderable/TextRenderable.h"

namespace Rival {

TextRenderable::TextRenderable(TextSpan /*span*/, TextProperties /*props*/, float /*x*/, float /*y*/) {}

TextRenderable::TextRenderable(
        std::vector<TextSpan> /*spans*/, const TextProperties /*props*/, float /*x*/, float /*y*/)
{
}

TextRenderable::TextRenderable(int /*maxChars*/, TextProperties /*props*/, float /*x*/, float /*y*/) {}

TextRenderable::TextRenderable(TextRenderable&& /*other*/) noexcept {}

TextRenderable::~TextRenderable() {}

}  // namespace Rival
