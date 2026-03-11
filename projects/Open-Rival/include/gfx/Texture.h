#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "gfx/Image.h"

namespace Rival {

enum class TextureFilterMode : std::uint8_t
{
    Nearest,
    Linear
};

enum class TextureWrapMode : std::uint8_t
{
    Repeat,
    ClampToEdge,
    MirroredRepeat
};

enum class TexturePixelFormat : std::uint8_t
{
    SingleByte,
    RGBA
};

struct TextureProperties
{
    /** Minification filter.
     * Generally, we have to stick to Nearest since we use indexed
     * textures, so interpolation between colors is impossible. */
    TextureFilterMode minFilter = TextureFilterMode::Nearest;

    /** Magnification filter.
     * Generally, we have to stick to Nearest since we use indexed
     * textures, so interpolation between colors is impossible. */
    TextureFilterMode magFilter = TextureFilterMode::Nearest;

    /** Texture wrapping mode. */
    TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge;

    /** Pixel format to use.
     * Most of our images use 1 byte per pixel since we use indexed colors.
     * Special images (like the palette itself) may use other formats. */
    TexturePixelFormat format = TexturePixelFormat::SingleByte;
};

class Texture
{
public:
    Texture(unsigned int id, int width, int height);

    unsigned int getId() const;

    int getWidth() const;

    int getHeight() const;

private:
    unsigned int id;
    int width;
    int height;
};

}  // namespace Rival
