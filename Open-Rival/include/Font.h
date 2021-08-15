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
    struct CharData {
        std::vector<float> txCoords;  // tx1, ty1, tx2, ty2 (between 0-1)
        glm::ivec2 size;              // Width / height of this FontChar (pixels)
        glm::ivec2 bearing;           // Offset from baseline to left / top of glyph (pixels)
        int advance;                  // Offset to advance to next glyph (pixels)
    };

    /**
     * Font that can be used to render characters.
     */
    class Font {
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
         * Creates a Font.
         */
        Font(Texture texture, std::map<char, CharData> chars);

        /**
         * Gets the Texture that backs this Font.
         */
        const Texture& getTexture() const;

        /**
         * Gets the data pertaining to the given character.
         */
        const CharData* getCharData(char c) const;

        /**
         * Loads the font with the given name.
         */
        static Font loadFont(std::string fontName);

    private:
        /**
         * Texture containing rendered bitmap characters.
         */
        Texture texture;

        /**
         * Data for each character in the font.
         */
        std::map<char, CharData> chars;

        static CharData makeChar(FT_GlyphSlot& glyph, int x, int imgWidth,
                int imgHeight);
        static void copyCharImage(FT_GlyphSlot& glyph, Image& target, int x);
        static std::vector<std::uint8_t> bitmapToVector(
                unsigned char* dataIn, int size);
    };

}  // namespace Rival

#endif  // FONTS_H
