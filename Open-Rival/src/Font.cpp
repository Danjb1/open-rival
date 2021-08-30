#include "pch.h"
#include "Font.h"

#include <freetype/ftfntfmt.h>
#include <freetype/ftwinfnt.h>
#include <gl/glew.h>

#include <algorithm>  // std::max
#include <memory>     // std::make_unique
#include <stdexcept>  // std::runtime_error, std::out_of_range
#include <utility>    // std::pair

#include "GLUtils.h"
#include "Resources.h"
#include "Texture.h"

namespace Rival {

    /**
     * Format identifier for Windows font files.
     *
     * The FreeType documentation surrounding these formats is poor, but there
     * is some more information in the Windows SDK (page 425).
     *
     * @see http://www.os2museum.com/files/docs/win10sdk/windows-1.03-sdk-prgref-1986.pdf
     */
    const std::string winFontFormat = "Windows FNT";

    // These are all the printable ASCII characters. Although we theoretically
    // support them, whether they can be rendered depends entirely on the font.
    // Note that the space character is a special case; we take the width from
    // the font, but it is skipped during rendering.
    const std::string Font::supportedChars =
            " !\"#$%&'()*+,-./1234567890:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

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

    FontFace::FontFace(FT_Library& ft, std::string filename) {
        if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
            throw FontLoadError("Failed to load font");
        }
    }

    FontFace::~FontFace() {
        FT_Done_Face(face);
    }

    FontLoadError::FontLoadError(const char* message)
        : std::runtime_error(message) {}

    Font Font::loadFont(
            FT_Library& ft,
            std::vector<std::string> fontDirs,
            std::string filename,
            int defaultSize) {
        for (std::string fontDir : fontDirs) {
            try {
                return loadFont(ft, fontDir + filename, defaultSize);
            } catch (const FontLoadError&) {
                // Try the next directory
                continue;
            }
        }

        throw std::runtime_error("Failed to load font: " + filename);
    }

    Font Font::loadFont(FT_Library& ft, std::string filename, int defaultSize) {
        FontFace faceWrapper(ft, filename);
        FT_Face& face = faceWrapper.face;

        // Check font format
        FT_Byte charOffset = 0;
        std::string format = std::string(FT_Get_Font_Format(face));
        if (format == winFontFormat) {
            FT_WinFNT_HeaderRec winFontHeader;
            if (FT_Get_WinFNT_Header(face, &winFontHeader)) {
                throw std::runtime_error("Failed to read Windows font header: "
                        + filename);
            }

            // Check the charset. MS Serif uses `CP1252`, which fortunately maps
            // directly to ASCII. Other fonts may require some more complex
            // mapping to find the correct characters.
            if (winFontHeader.charset != FT_WinFNT_ID_CP1252) {
                throw std::runtime_error("Unsupported charset in font: "
                        + filename);
            }

            // The documentation of this field is poor, but it seems as though
            // Windows fonts can have all characters offset by some amount.
            // This might be a horrible hack, but it seems to work for now.
            charOffset = winFontHeader.first_char - 1;
        }

        // Set font size. Using zero width means it will be auto-calculated
        // based on the height.
        FT_Set_Pixel_Sizes(face, 0, fontHeight);

        GLsizei imgWidth = 0;
        GLsizei imgHeight = 0;

        // Determine the image size
        for (size_t i = 0; i < Font::supportedChars.length(); ++i) {
            unsigned char c = Font::supportedChars[i];

            if (c == ' ') {
                // Spaces are not part of the Font's texture
                continue;
            }

            // Map our desired character to its index in the font
            unsigned char charCode = getCharCode(c, charOffset);

            // Load this character into the `face->glyph` slot
            if (FT_Load_Char(face, charCode, FT_LOAD_RENDER)) {
                throw std::runtime_error("Failed to load character: " + c);
            }

            if (!face->glyph->bitmap.buffer) {
                // Glyph is not present in font; character will be displayed as
                // an empty space.
                std::cout << "Font "
                          << filename
                          << " does not support character "
                          << makePrintable(c)
                          << "\n";
            }

            int charWidth = face->glyph->bitmap.width;
            int charHeight = face->glyph->bitmap.rows;

            imgWidth += charWidth + 2 * Font::charPadding;
            imgHeight = std::max(imgHeight, charHeight);
        }

        Image fontBitmap(imgWidth, imgHeight, 0);
        std::map<char, CharData> chars;
        int nextX = Font::charPadding;

        // Create our characters
        for (size_t i = 0; i < Font::supportedChars.length(); ++i) {
            unsigned char c = Font::supportedChars[i];
            unsigned char charCode = c - charOffset;

            // It's unfortunate that we are loading every character twice, but
            // we need two passes to correctly determine the image dimensions.
            // The alternative would be to guess the image size and then crop
            // it afterwards, or copy each char to memory after the first load.
            if (FT_Load_Char(face, charCode, FT_LOAD_RENDER)) {
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
        TextureProperties props = createTextureProperties(face);
        Texture tex = Texture::wrap(std::move(fontBitmap), props);

        return Font(tex, chars, defaultSize);
    }

    /**
     * Gets the index of `c` within a font whose characters are offset by
     * `charOffset`.
     */
    unsigned char Font::getCharCode(unsigned char c, FT_Byte charOffset) {
        return c - charOffset;
    }

    /**
     * Converts `c` to an int because some characters are not printable.
     */
    int Font::makePrintable(unsigned char c) {
        return static_cast<int>(c);
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
        std::vector<std::uint8_t> data = bitmapToVector(glyph->bitmap);
        Image src(charWidth, charHeight,
                std::make_unique<std::vector<std::uint8_t>>(data));
        Image::copyImage(src, target, x, 0);
    }

    /**
     * Converts bitmap data for a glyph into an equivalent vector.
     *
     * The resulting vector uses one byte to represent each pixel.
     */
    std::vector<std::uint8_t> Font::bitmapToVector(FT_Bitmap& bmp) {
        switch (bmp.pixel_mode) {
        case FT_Pixel_Mode::FT_PIXEL_MODE_MONO:
            return monoBitmapToVector(bmp);
        case FT_Pixel_Mode::FT_PIXEL_MODE_GRAY:
            return grayBitmapToVector(bmp);
        default:
            throw std::runtime_error("Unsupported font format");
        }
    }

    std::vector<std::uint8_t> Font::monoBitmapToVector(FT_Bitmap& bmp) {
        // One bit per pixel - needs some tinkering
        int charWidth = bmp.width;
        int charHeight = bmp.rows;
        int size = charWidth * charHeight;
        std::vector<std::uint8_t> vecData;
        vecData.reserve(size);

        int bitIdx = 0;
        for (int y = 0; y < charHeight; ++y) {
            for (int x = 0; x < charWidth; ++x) {

                // Find the byte containing our desired bit
                int byteIdx = bitIdx / 8;
                unsigned char containingByte = bmp.buffer[byteIdx];

                // Find our bit within the byte
                int relativeBitIdx = bitIdx % 8;
                int bitShift = 7 - relativeBitIdx;
                std::uint8_t bitVal = (containingByte >> bitShift) & 1;

                // Resulting value is either 0 or 255 (invisible or opaque)
                vecData.push_back(std::uint8_t(bitVal * 0xff));

                ++bitIdx;
            }

            // Skip any padding at the end of the row
            // (pitch is the width + row padding, in bytes)
            bitIdx += (bmp.pitch * 8) - charWidth;
        }

        return vecData;
    }

    std::vector<std::uint8_t> Font::grayBitmapToVector(FT_Bitmap& bmp) {
        // One byte per pixel - we can directly copy the original buffer
        int charWidth = bmp.width;
        int charHeight = bmp.rows;
        int size = charWidth * charHeight;
        std::uint8_t* data = static_cast<std::uint8_t*>(bmp.buffer);
        return std::vector<std::uint8_t>(data, data + size);
    }

    TextureProperties Font::createTextureProperties(const FT_Face& face) {
        TextureProperties props;
        if (face->glyph->bitmap.pixel_mode == FT_Pixel_Mode::FT_PIXEL_MODE_MONO) {
            // For 1-bit fonts, use nearest neighbor interpolation otherwise
            // they become blurry
            props.minFilter = GL_NEAREST;
            props.magFilter = GL_NEAREST;
        } else {
            // For regular fonts, use linear interpolation so that we can
            // upscale or downscale the font without it looking terrible
            props.minFilter = GL_LINEAR;
            props.magFilter = GL_LINEAR;
        }
        return props;
    }

}  // namespace Rival
