#ifndef FONTS_H
#define FONTS_H

#include <map>
#include <string>

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "TextureAtlas.h"

namespace Rival {

    struct FontChar {
        float txCoordX1;     // x-position of the left edge of this FontChar within the containing texture (0-1)
        float txCoordX2;     // x-position of the right edge of this FontChar within the containing texture (0-1)
        glm::ivec2 size;     // Width / height of this FontChar
        glm::ivec2 bearing;  // Offset from baseline to left / top of glyph
        int advance;         // Offset to advance to next glyph
    };

    class Font {
    public:
        static const std::string supportedChars;

        /**
         * Padding around each character in the font.
         *
         * We only add padding horizontally; there is no need for any vertical
         * padding as we only use a single row.
         */
        static const int charPadding = 1;

        Font(Texture texture, std::map<char, FontChar> chars);

        const Texture& getTexture() const;

        const FontChar* getCharData(char c) const;

        static Font loadFont(std::string fontName);

    private:
        /**
         * Texture containing rendered bitmap characters.
         */
        Texture texture;

        /**
         * Glyphs for each character in the font.
         */
        std::map<char, FontChar> chars;
    };

}  // namespace Rival

#endif  // FONTS_H
