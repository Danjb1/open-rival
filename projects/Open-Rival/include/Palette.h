#pragma once

#include <array>
#include <cstdint>

namespace Rival { namespace Palette {

// Number of colors in the palette
const int paletteSize = 256;

// Number of channels per color of the palette
const int paletteChannels = 4;

// Number of bytes required to store the palette
const int paletteBytes = paletteSize * paletteChannels;

using Palette = std::array<std::uint32_t, paletteSize>;

extern Palette paletteGame;
extern Palette paletteTitle;
extern Palette paletteLoading;
extern Palette paletteMenu;
extern Palette paletteHireTroops;

}}  // namespace Rival::Palette