#pragma once

#include <array>
#include <cstdint>

namespace Rival { namespace Palette {

// Number of colors in the palette
constexpr int numColors = 256;

// Number of channels per color of the palette (RGBA)
constexpr int numChannels = 4;

// Number of bytes required for a single palette
constexpr int numBytesSinglePalette = numColors * numChannels;

using Palette = std::array<std::uint32_t, numBytesSinglePalette>;

extern Palette paletteGame;
extern Palette paletteTitle;
extern Palette paletteLoading;
extern Palette paletteMenu;
extern Palette paletteHireTroops;

}}  // namespace Rival::Palette
