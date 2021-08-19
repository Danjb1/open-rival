#include "pch.h"
#include "Font.h"

#include <gl/glew.h>

#include <algorithm>  // std::max
#include <memory>     // std::make_unique
#include <stdexcept>  // std::runtime_error, std::out_of_range
#include <utility>    // std::pair

#include "GLUtils.h"
#include "Resources.h"
#include "Texture.h"

namespace Rival {

    // These are all the printable ASCII characters. Although we theoretically
    // support them, whether they can be rendered depends entirely on the font.
    // Note that the space character is a special case; we take the width from
    // the font, but it is skipped during rendering.
    const std::string Font::supportedChars =
            " !\"#$%&'()*+,-./"
            "1234567890"
            ":;<=>?@"
            "ABCDEFGHJKLMNOPQRSTUVWXYZ"
            "[\\]^_`"
            "abcdefghijklmnopqrstuvwxyz"
            "{|}~";

    Font::Font(Texture texture, std::map<char, CharData> chars, int defaultSize)
        : texture(texture),
          chars(chars),
          defaultSize(defaultSize) {
    }

    const CharData* Font::getCharData(char c) const {
        try {
            return &chars.at(c);
        } catch (const std::out_of_range&) {
            return nullptr;
        }
    }

    /**
     * Loads a TrueType font and produces a Font object backed by a texture.
     *
     * This texture contains every supported character in a single row, with
     * padding between them.
     *
     * If the font does not contain a glyph for one of our supported characters,
     * then that character will be displayed as an empty space.
     *
     * @see https://learnopengl.com/In-Practice/Text-Rendering
     */
    Font Font::loadFont(std::string fontName, int defaultSize) {
        // Initialize FreeType
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("Failed to initialize FreeType library");
        }

        // Load font
        FT_Face face;
        std::string filename = Resources::fontDir + fontName;
        if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
            throw std::runtime_error("Failed to load font: " + fontName);
        }

        // Set font size. Using zero width means it will be auto-calculated
        // based on the height.
        FT_Set_Pixel_Sizes(face, 0, fontHeight);

        std::map<char, CharData> chars;
        GLsizei imgWidth = 0;
        GLsizei imgHeight = 0;

        // Determine the image size
        for (size_t i = 0; i < Font::supportedChars.length(); ++i) {
            char c = Font::supportedChars[i];

            if (c == ' ') {
                // Spaces are not part of the Font's texture
                continue;
            }

            // Load this character into the `face->glyph` slot
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                throw std::runtime_error("Failed to load character: " + c);
            }

            if (!face->glyph->bitmap.buffer) {
                // Glyph is not present in font; character will be displayed as
                // an empty space.
                std::cout << "Font "
                          << fontName
                          << " does not support character "
                          // Non-ASCII characters are not printed correctly,
                          // so we print the int value instead. But first we
                          // make it unsigned otherwise we can end up with
                          // negative numbers.
                          << static_cast<int>(static_cast<unsigned char>(c))
                          << "\n ";
            }

            int charWidth = face->glyph->bitmap.width;
            int charHeight = face->glyph->bitmap.rows;

            imgWidth += charWidth + 2 * Font::charPadding;
            imgHeight = std::max(imgHeight, charHeight);
        }

        // Create an image to hold all of our characters
        Image fontBitmap(imgWidth, imgHeight, 0);

        int nextX = Font::charPadding;

        // Create our characters
        for (size_t i = 0; i < Font::supportedChars.length(); ++i) {
            char c = Font::supportedChars[i];

            // It's unfortunate that we are loading every character twice, but
            // we need two passes to correctly determine the image dimensions.
            // The alternative would be to guess the image size and then crop
            // it afterwards, or copy each char to memory after the first load.
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                throw std::runtime_error("Failed to load character: " + c);
            }

            // Store this character in the font
            CharData charData =
                    makeChar(face->glyph, nextX, imgWidth, imgHeight);
            chars.insert(std::pair<char, CharData>(c, charData));

            if (c == ' ') {
                // Spaces are not part of the Font's texture
                continue;
            }

            // Copy bitmap data to our Image
            copyCharImage(face->glyph, fontBitmap, nextX);

            int charWidth = face->glyph->bitmap.width;
            nextX += charWidth + 2 * Font::charPadding;
        }

        // Disable byte-alignment restriction as we only need 1 byte per pixel
        GLUtils::PixelStore byteAlignment(GLUtils::PackAlignment::BYTES_1);

        // Generate texture to hold this font
        TextureProperties props;
        // We want to use interpolation here so that we can upscale or downscale
        // the font without it looking terrible
        props.minFilter = GL_LINEAR;
        props.magFilter = GL_LINEAR;
        Texture tex = Texture::wrap(std::move(fontBitmap), props);

        // Free resources
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        return Font(tex, chars, defaultSize);
    }

    /**
     * Makes a CharData from a glyph.
     *
     * @param glyph The glyph.
     * @param x The x-position of the glyph within the font bitmap.
     * @param imgWidth The width of the font bitmap.
     * @param imgHeight The height of the font bitmap.
     */
    CharData Font::makeChar(
            FT_GlyphSlot& glyph, int x, int imgWidth, int imgHeight) {
        int charWidth = glyph->bitmap.width;
        int charHeight = glyph->bitmap.rows;

        float tx1 = static_cast<float>(x) / imgWidth;
        float ty1 = 0;
        float tx2 = static_cast<float>(x + charWidth) / imgWidth;
        float ty2 = static_cast<float>(charHeight) / imgHeight;
        std::vector<float> texCoords = { tx1, ty1, tx2, ty2 };

        auto charSize = glm::ivec2(charWidth, charHeight);

        auto bearing = glm::ivec2(
                glyph->bitmap_left,
                glyph->bitmap_top);

        // Bit shift because the advance is given in 1/64 pixels
        int advance = static_cast<int>(glyph->advance.x >> 6);

        return { texCoords, charSize, bearing, advance };
    }

    /**
     * Copies the bitmap data for a glyph into a target image.
     */
    void Font::copyCharImage(FT_GlyphSlot& glyph, Image& target, int x) {
        int charWidth = glyph->bitmap.width;
        int charHeight = glyph->bitmap.rows;
        int size = charWidth * charHeight;
        std::vector<std::uint8_t> data =
                bitmapToVector(glyph->bitmap.buffer, size);
        Image src(charWidth, charHeight,
                std::make_unique<std::vector<std::uint8_t>>(data));
        Image::copyImage(src, target, x, 0);
    }

    /**
     * Converts bitmap data for a glyph into an equivalent vector.
     */
    std::vector<std::uint8_t> Font::bitmapToVector(
            unsigned char* dataIn, int size) {
        std::uint8_t* data = static_cast<std::uint8_t*>(dataIn);
        return std::vector<std::uint8_t>(data, data + size);
    }

}  // namespace Rival
