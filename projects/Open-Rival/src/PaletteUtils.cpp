#include "pch.h"

#include "PaletteUtils.h"

#include <gl/glew.h>

#include <array>
#include <cstdint>

#include "Palette.h"
#include "Texture.h"

namespace Rival { namespace PaletteUtils {

// Number of bytes required for all palettes
constexpr int paletteTextureBytes = Palette::numBytesSinglePalette * numPalettes;

// Player color info
constexpr int numColorsPerPlayer = 6;
constexpr int p1ColorsIndex = 160;
constexpr int p2ColorsIndex = p1ColorsIndex + numColorsPerPlayer;

void addColorToPalette(std::array<std::uint8_t, paletteTextureBytes>& data, std::uint32_t col, std::size_t& nextIndex)
{
    // RGBA
    data[nextIndex] = static_cast<std::uint8_t>((col & 0xff000000) >> 24);
    data[nextIndex + 1] = static_cast<std::uint8_t>((col & 0x00ff0000) >> 16);
    data[nextIndex + 2] = static_cast<std::uint8_t>((col & 0x0000ff00) >> 8);
    data[nextIndex + 3] = static_cast<std::uint8_t>(col & 0x000000ff);

    nextIndex += Palette::numChannels;
}

std::shared_ptr<const Texture> createPaletteTexture()
{
    std::array<std::uint8_t, paletteTextureBytes> data { 0 };
    std::size_t nextIndex = 0;

    // 1 palette per player...
    // Since we are using indexed textures, the only way to render units in different colours is to change the palette.
    // So, we create a separate palette for each team, overwriting player 1's colours in the palette each time.
    for (int player = 0; player < PlayerStore::maxPlayers; ++player)
    {
        // This is the offset of THIS player's colors in the game palette, from the start of the "player colors" section
        int playerColorOffset = player * numColorsPerPlayer;

        for (int i = 0; i < Palette::numColors; ++i)
        {
            std::uint32_t col;

            if (i >= p1ColorsIndex && i < p2ColorsIndex)
            {
                col = Palette::paletteGame[i + playerColorOffset];
            }
            else
            {
                col = Palette::paletteGame[i];
            }

            addColorToPalette(data, col, nextIndex);
        }
    }

    // Title screen
    for (int i = 0; i < Palette::numColors; ++i)
    {
        addColorToPalette(data, Palette::paletteTitle[i], nextIndex);
    }

    // Loading screen
    for (int i = 0; i < Palette::numColors; ++i)
    {
        addColorToPalette(data, Palette::paletteLoading[i], nextIndex);
    }

    // Menu
    for (int i = 0; i < Palette::numColors; ++i)
    {
        addColorToPalette(data, Palette::paletteMenu[i], nextIndex);
    }

    // Hire troops
    for (int i = 0; i < Palette::numColors; ++i)
    {
        addColorToPalette(data, Palette::paletteHireTroops[i], nextIndex);
    }

    int texWidth = Palette::numColors;
    int texHeight = numPalettes;

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    return std::make_shared<const Texture>(textureId, texWidth, texHeight);
}

float getPaletteTxY(int paletteIndex)
{
    return static_cast<float>(paletteIndex) / numPalettes;
}

}}  // namespace Rival::PaletteUtils
