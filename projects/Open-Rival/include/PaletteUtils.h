#pragma once

#include <memory>

namespace Rival {

class Texture;

namespace PaletteUtils {

std::shared_ptr<const Texture> createPaletteTexture();

}
}  // namespace Rival
