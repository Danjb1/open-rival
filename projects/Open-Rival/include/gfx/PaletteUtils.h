#pragma once

#include <glm/vec4.hpp>

#include <cstdint>
#include <memory>
#include <string>

#include "game/PlayerState.h"

namespace Rival {

class Renderer;
class Texture;

namespace PaletteUtils {

/** Creates the combined palette texture used by the game. */
std::shared_ptr<const Texture> createPaletteTexture(Renderer* renderer);

/** Gets the palette index for the palette with the given name (game, title, loading, menu or hire_troops). */
int getPaletteIndex(const std::string& name);

/** Gets the texture co-ordinate (y) for the given palette index. */
float getPaletteTxY(int paletteIndex);

/** Converts an RGBA hex value (e.g. from a Palette) to a colour with components in the range 0-1. */
glm::vec4 fromHex(std::uint32_t hexValue);

// Palette index constants
constexpr int paletteIndexGame = 0;
constexpr int paletteIndexTitle = PlayerStore::maxPlayers;
constexpr int paletteIndexLoading = paletteIndexTitle + 1;
constexpr int paletteIndexMenu = paletteIndexLoading + 1;
constexpr int paletteIndexHireTroops = paletteIndexMenu + 1;
constexpr int numPalettes = paletteIndexHireTroops + 1;

}  // namespace PaletteUtils
}  // namespace Rival
