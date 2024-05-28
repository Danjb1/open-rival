#pragma once

#include <array>
#include <cstdint>

namespace Rival { namespace Palette {

// Number of colors in the palette
constexpr int numColors = 256;

// Number of colors in the palette
constexpr int maxIndex = numColors - 1;

// Number of channels per color of the palette (RGBA)
constexpr int numChannels = 4;

// Number of bytes required for a single palette
constexpr int numBytesSinglePalette = numColors * numChannels;

// Palette lookup (0-1) used for global transparency
constexpr float globalTransparentColor = 255.f / maxIndex;

// Palette lookup (0-1) used for cursor transparency
constexpr float cursorTransparentColor = 0.f / maxIndex;

using Palette = std::array<std::uint32_t, numBytesSinglePalette>;

extern Palette paletteGame;
extern Palette paletteTitle;
extern Palette paletteLoading;
extern Palette paletteMenu;
extern Palette paletteHireTroops;

}}  // namespace Rival::Palette
