#pragma once

#include <cstdint>
#include <memory>

#include "game/PlayerState.h"

namespace Rival {

class Texture;

namespace PaletteUtils {

/** Creates the combined palette texture used by the game. */
std::shared_ptr<const Texture> createPaletteTexture();

/** Gets the texture co-ordinate (y) for the given palette index. */
float getPaletteTxY(int paletteIndex);

std::vector<float> fromHex(std::uint32_t hexValue);

// Palette index constants
constexpr int paletteIndexGame = 0;
constexpr int paletteIndexTitle = PlayerStore::maxPlayers;
constexpr int paletteIndexLoading = paletteIndexTitle + 1;
constexpr int paletteIndexMenu = paletteIndexLoading + 1;
constexpr int paletteIndexHireTroops = paletteIndexMenu + 1;
constexpr int numPalettes = paletteIndexHireTroops + 1;

}  // namespace PaletteUtils
}  // namespace Rival
