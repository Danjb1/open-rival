#include "pch.h"

#include "PaletteUtils.h"

#include <gl/glew.h>

#include <array>
#include <cstdint>

#include "Palette.h"

namespace Rival { namespace PaletteUtils {

Texture createPaletteTexture()
{
    std::array<std::uint8_t, Palette::numBytesCombinedPalettes> data { 0 };

    // Since we are using indexed textures, the only way to render units in different colours is to change the palette.
    // So, we create a separate palette for each team, overwriting player 1's colours in the palette each time.
    for (int player = 0; player < PlayerStore::maxPlayers; ++player)
    {
        int playerPaletteStart = player * Palette::numBytesSinglePalette;
        int playerColorOffset = player * Palette::numColorsPerPlayer;

        for (int i = 0; i < Palette::numColors; ++i)
        {
            int start = playerPaletteStart + (i * Palette::numChannels);
            std::uint32_t col;

            if (i >= Palette::p1ColorsIndex && i < Palette::p2ColorsIndex)
            {
                col = Palette::paletteGame[i + playerColorOffset];
            }
            else
            {
                col = Palette::paletteGame[i];
            }

            // RGBA
            data[start] = static_cast<std::uint8_t>((col & 0xff000000) >> 24);
            data[start + 1] = static_cast<std::uint8_t>((col & 0x00ff0000) >> 16);
            data[start + 2] = static_cast<std::uint8_t>((col & 0x0000ff00) >> 8);
            data[start + 3] = static_cast<std::uint8_t>(col & 0x000000ff);
        }
    }

    int texWidth = Palette::numColors;
    int texHeight = PlayerStore::maxPlayers;

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture(textureId, texWidth, texHeight);
}

}}  // namespace Rival::PaletteUtils
