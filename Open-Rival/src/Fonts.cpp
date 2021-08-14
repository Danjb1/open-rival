#include "pch.h"
#include "Fonts.h"

#include <algorithm>  // std::max
#include <memory>     // std::make_unique
#include <stdexcept>  // std::runtime_error, std::out_of_range
#include <utility>    // std::pair

#include "GLUtils.h"
#include "Image.h"
#include "Resources.h"
#include "Texture.h"

namespace Rival {

    const std::string Font::supportedChars =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHJKLMNOPQRSTUVWXYZ"
            "1234567890";

    Font::Font(Texture texture, std::map<char, FontChar> chars)
        : texture(texture),
          chars(chars) {
    }

    const Texture& Font::getTexture() const {
        return texture;
    }

    const FontChar* Font::getCharData(char c) const {
        try {
            return &chars.at(c);
        } catch (const std::out_of_range&) {
            return nullptr;
        }
    }

    /**
     * Loads a TrueType font and produces a Font object backed by a texture
     * atlas.
     *
     * @see https://learnopengl.com/In-Practice/Text-Rendering
     */
    Font Font::loadFont(std::string fontName) {
        // Initialize FreeType
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("Failed to initialize FreeType library");
        }

        // Load font
        FT_Face face;
        std::string filename = Resources::fontDir + fontName;
        if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
            throw std::runtime_error("Failed to load font");
        }

        // Set font size. Using zero width means it will be auto-calculated
        // based on the height.
        FT_Set_Pixel_Sizes(face, 0, 48);

        std::map<char, FontChar> chars;
        GLsizei imgWidth = 0;
        GLsizei imgHeight = 0;

        // Determine the image size
        for (size_t i = 0; i < Font::supportedChars.length(); ++i) {
            char c = Font::supportedChars[i];

            // Load this character into the `face->glyph` slot
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                throw std::runtime_error("Failed to load character");
            }

            int charWidth = face->glyph->bitmap.width;
            int charHeight = face->glyph->bitmap.rows;

            imgWidth += charWidth + 2 * Font::charPadding;
            imgHeight = std::max(imgHeight, charHeight);
        }

        // Create an image to hold all of our characters
        Image fontBitmap(imgWidth, imgHeight);

        int nextX = Font::charPadding;

        // Create our characters
        for (size_t i = 0; i < Font::supportedChars.length(); ++i) {
            char c = Font::supportedChars[i];

            // It's unfortunate that we are loading every character twice, but
            // we need two passes to correctly determine the image dimensions.
            // The alternative would be to guess the image size and then crop
            // it afterwards.
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                throw std::runtime_error("Failed to load character");
            }

            int charWidth = face->glyph->bitmap.width;
            int charHeight = face->glyph->bitmap.rows;
            float tx1 = static_cast<float>(nextX) / imgWidth;
            float tx2 = static_cast<float>(nextX + charWidth) / imgWidth;

            // Store this character in the font
            FontChar fontChar = {
                tx1,
                tx2,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<int>(face->glyph->advance.x)
            };
            chars.insert(std::pair<char, FontChar>(c, fontChar));

            // Copy bitmap data to our Image
            std::uint8_t* buffer = static_cast<std::uint8_t*>(
                    face->glyph->bitmap.buffer);
            int size = charWidth * charHeight;
            std::vector<std::uint8_t> data(buffer, buffer + size);
            Image src(charWidth, charHeight,
                    std::make_unique<std::vector<std::uint8_t>>(data));
            Image::copyImage(src, fontBitmap, nextX, 0);

            nextX += charWidth + 2 * Font::charPadding;
        }

        // Disable byte-alignment restriction as we only need 1 byte per pixel
        GLUtils::PixelStore byteAlignment(GLUtils::PackAlignment::BYTES_1);

        // Generate texture to hold this font
        Texture tex = Texture::wrap(std::move(fontBitmap));

        // Free resources
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        return Font(tex, chars);
    }

}  // namespace Rival
