#ifndef FONTS_H
#define FONTS_H

#include <map>
#include <string>

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Image.h"
#include "TextureAtlas.h"

namespace Rival {

    /**
     * Data relating to a character within a Font.
     */
    struct CharData
    {
        std::vector<float> txCoords;  // tx1, ty1, tx2, ty2 (between 0-1)
        glm::ivec2 size;              // Width / height of this FontChar (pixels)
        glm::ivec2 bearing;           // Offset from baseline to left / top of glyph (pixels)
        int advance;                  // Offset to advance to next glyph (pixels)
    };

    /**
     * Wrapper around an `FT_Face` that uses RAII.
     */
    class FontFace
    {
    public:
        FT_Face face;

        FontFace(FT_Library& ft, std::string filename);
        ~FontFace();
    };

    /**
     * Exception thrown when a Font cannot be loaded.
     */
    class FontLoadError : public std::runtime_error
    {
    public:
        FontLoadError(const char* message);
    };

    /**
     * Font that can be used to render characters.
     */
    class Font
    {
    public:
        /**
         * Characters that can be rendered using this font.
         */
        static const std::string supportedChars;

        /**
         * Horizontal padding around each character in the font.
         *
         * There is no need for any vertical padding as the characters are all
         * in a single row, so there is no danger of any texture bleeding.
         */
        static const int charPadding = 1;

        /**
         * Font height to use when generating the texture.
         *
         * It is best to use the highest value we will need here so that the
         * font can be rendered at any size later without the quality suffering.
         */
        static const int fontHeight = 32;

        /**
         * Creates a Font.
         */
        Font(Texture texture, std::map<char, CharData> chars, int defaultSize);

        /**
         * Gets the Texture that backs this Font.
         */
        const Texture& getTexture() const
        {
            return texture;
        }

        /**
         * Gets the data pertaining to the given character.
         */
        const CharData* getCharData(char c) const;

        /**
         * Gets the data pertaining to the given character.
         */
        const int getDefaultSize() const
        {
            return defaultSize;
        }

        /**
         * Loads a font and produces a Font object backed by a texture.
         *
         * This texture contains every supported character in a single row, with
         * padding between them.
         *
         * If the font does not contain a glyph for one of our supported
         * characters, then that character will be displayed as an empty space.
         *
         * @see https://learnopengl.com/In-Practice/Text-Rendering
         */
        static Font loadFont(FT_Library& ft, std::vector<std::string> fontDirs, std::string fontName, int defaultSize);

    private:
        /**
         * Texture containing rendered bitmap characters.
         */
        Texture texture;

        /**
         * Data for each character in the font.
         */
        std::map<char, CharData> chars;

        /**
         * The size at which the font is rendered when scale = 1.
         */
        int defaultSize;

        static Font loadFont(FT_Library& ft, std::string filename, int defaultSize);

        static unsigned char getCharCode(unsigned char c, FT_Byte charOffset);
        static inline int makePrintable(unsigned char c);
        static CharData makeChar(FT_GlyphSlot& glyph, int x, int imgWidth, int imgHeight);
        static void copyCharImage(FT_GlyphSlot& glyph, Image& target, int x);
        static std::vector<std::uint8_t> bitmapToVector(FT_Bitmap& bmp);
        static std::vector<std::uint8_t> monoBitmapToVector(FT_Bitmap& bmp);
        static std::vector<std::uint8_t> grayBitmapToVector(FT_Bitmap& bmp);
        static TextureProperties createTextureProperties(const FT_Face& face);
    };

}  // namespace Rival

#endif  // FONTS_H
