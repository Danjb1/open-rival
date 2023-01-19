#pragma once

#include <memory>

namespace Rival {

class Texture;

namespace PaletteUtils {

std::unique_ptr<Texture> createPaletteTexture();

}
}  // namespace Rival
