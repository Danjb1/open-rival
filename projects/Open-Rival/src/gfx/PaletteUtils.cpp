#include "gfx/PaletteUtils.h"

#include "gfx/GlewWrapper.h"

#include <cstdint>
#include <vector>

#include "gfx/Palette.h"
#include "gfx/Texture.h"
#include "utils/MathUtils.h"

namespace Rival { namespace PaletteUtils {

static const int texWidth = Palette::numColors;
static const int texHeight = MathUtils::nextPowerOf2(numPalettes);

// Player color info
static constexpr int numColorsPerPlayer = 6;
static constexpr int p1ColorsIndex = 160;
static constexpr int p2ColorsIndex = p1ColorsIndex + numColorsPerPlayer;

void addColorToPalette(std::vector<std::uint8_t>& data, std::uint32_t col)
{
    // RGBA
    data.push_back(static_cast<std::uint8_t>((col & 0xff000000) >> 24));
    data.push_back(static_cast<std::uint8_t>((col & 0x00ff0000) >> 16));
    data.push_back(static_cast<std::uint8_t>((col & 0x0000ff00) >> 8));
    data.push_back(static_cast<std::uint8_t>(col & 0x000000ff));
}

std::shared_ptr<const Texture> createPaletteTexture()
{
    // Number of bytes required for all palettes
    const int paletteTextureBytes = Palette::numBytesSinglePalette * texHeight;

    std::vector<std::uint8_t> data;
    data.reserve(paletteTextureBytes);

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

            addColorToPalette(data, col);
        }
    }

    // Title screen
    for (int i = 0; i < Palette::numColors; ++i)
    {
        addColorToPalette(data, Palette::paletteTitle[i]);
    }

    // Loading screen
    for (int i = 0; i < Palette::numColors; ++i)
    {
        addColorToPalette(data, Palette::paletteLoading[i]);
    }

    // Menu
    for (int i = 0; i < Palette::numColors; ++i)
    {
        addColorToPalette(data, Palette::paletteMenu[i]);
    }

    // Hire troops
    for (int i = 0; i < Palette::numColors; ++i)
    {
        addColorToPalette(data, Palette::paletteHireTroops[i]);
    }

    // Default-initialize any remaining elements to fill out the texture
    data.resize(paletteTextureBytes);

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
    return static_cast<float>(paletteIndex) / texHeight;
}

GLfloat byteToFloat(std::uint8_t byte)
{
    return static_cast<GLfloat>(byte) / 255.f;
}

std::vector<GLfloat> fromHex(std::uint32_t hexValue)
{
    // Extract each byte from the hexadecimal value
    std::uint8_t byte1 = (hexValue >> 24) & 0xFF;
    std::uint8_t byte2 = (hexValue >> 16) & 0xFF;
    std::uint8_t byte3 = (hexValue >> 8) & 0xFF;
    std::uint8_t byte4 = hexValue & 0xFF;

    // Convert each byte to a float value
    return { byteToFloat(byte1), byteToFloat(byte2), byteToFloat(byte3), byteToFloat(byte4) };
}

}}  // namespace Rival::PaletteUtils
