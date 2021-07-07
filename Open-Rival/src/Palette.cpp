#include "pch.h"
#include "Palette.h"

#include <array>

namespace Rival {
namespace Palette {

    Texture createPaletteTexture() {

        // Create palette texture
        std::array<unsigned char, paletteBytes> data;

        for (int i = 0; i < paletteSize; i++) {
            int start = i * paletteChannels;
            uint32_t col = palette[i];
            // RGBA
            data[start] = static_cast<unsigned char>((col & 0xff000000) >> 24);
            data[start + 1] = static_cast<unsigned char>((col & 0x00ff0000) >> 16);
            data[start + 2] = static_cast<unsigned char>((col & 0x0000ff00) >> 8);
            data[start + 3] = static_cast<unsigned char>(col & 0x000000ff);
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

        return Texture(textureId, paletteSize, 1);
    }

}}  // namespace Rival::Palette
