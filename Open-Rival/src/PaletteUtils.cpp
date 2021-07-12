#include "pch.h"
#include "PaletteUtils.h"

#include <gl/glew.h>
#include <array>
#include <cstdint>

#include "Palette.h"

namespace Rival {
namespace PaletteUtils {

    Texture createPaletteTexture() {

        // Create palette texture
        std::array<std::uint8_t, Palette::paletteBytes> data { 0 };

        for (int i = 0; i < Palette::paletteSize; i++) {
            int start = i * Palette::paletteChannels;
            std::uint32_t col = Palette::paletteGame[i];
            // RGBA
            data[start] = static_cast<std::uint8_t>((col & 0xff000000) >> 24);
            data[start + 1] = static_cast<std::uint8_t>((col & 0x00ff0000) >> 16);
            data[start + 2] = static_cast<std::uint8_t>((col & 0x0000ff00) >> 8);
            data[start + 3] = static_cast<std::uint8_t>(col & 0x000000ff);
        }

        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1,
                0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        return Texture(textureId, Palette::paletteSize, 1);
    }

}}  // namespace Rival::PaletteUtils
