#pragma once

#include <array>
#include <cstdint>

#include "PlayerState.h"

namespace Rival { namespace Palette {

// Number of colors in the palette
const int numColors = 256;

// Number of channels per color of the palette (RGBA)
const int numChannels = 4;

// Number of bytes required for a single palette
const int numBytesSinglePalette = numColors * numChannels;

// Number of bytes required for all palettes (1 per player)
const int numBytesCombinedPalettes = numBytesSinglePalette * PlayerStore::maxPlayers;

// Player color info
const int numColorsPerPlayer = 6;
const int p1ColorsIndex = 160;
const int p2ColorsIndex = p1ColorsIndex + numColorsPerPlayer;

using Palette = std::array<std::uint32_t, numBytesSinglePalette>;

extern Palette paletteGame;
extern Palette paletteTitle;
extern Palette paletteLoading;
extern Palette paletteMenu;
extern Palette paletteHireTroops;

}}  // namespace Rival::Palette
