#ifndef TEXT_RENDERABLE_H
#define TEXT_RENDERABLE_H

#include <string>

#include <glm/glm.hpp>

#include "Color.h"
#include "Font.h"
#include "Spritesheet.h"

namespace Rival {

    /**
     * A portion of text with a configurable color.
     */
    struct TextSpan {
        std::string text;
        Color color;
    };

    /**
     * Properties that control how some text should be rendered.
     */
    struct TextProperties {
        /**
         * Font to use when rendering the text.
         */
        const Font& font;

        /**
         * Scale at which to render the text.
         *
         * This is a fraction of the font size used to create the Font's texture
         * atlas. Quality may suffer at scales > 1 as the texture will have to
         * stretch.
         */
        float scale = 1.0f;
    };

    /**
     * Class that allows colored text to be rendered as textured quads.
     *
     * This creates the VAO and all necessary buffers.
     */
    class TextRenderable {

    public:
        static const int numVertexDimensions = 3;    // x, y, z
        static const int numTexCoordDimensions = 2;  // u, v
        static const int numColorDimensions = 3;     // r, g, b
        static const int numVerticesPerChar = 4;

        static const Color defaultColor;
        static const Color highlightColor;

        /*
         * 6 indices are required to render a quad using GL_TRIANGLES:
         *  - First triangle: 0-1-2
         *  - Second triangle: 2-3-0
         */
        static const int numIndicesPerChar = 6;

        mutable bool dirty = true;

        /**
         * Constructs a TextRenderable.
         */
        TextRenderable(
                std::vector<TextSpan> spans,
                TextProperties props,
                float x,
                float y);

        // Disable moving / copying
        TextRenderable(const TextRenderable& other) = delete;
        TextRenderable(TextRenderable&& other) = delete;
        TextRenderable& operator=(const TextRenderable& other) = delete;
        TextRenderable& operator=(TextRenderable&& other) = delete;

        GLuint getVao() const { return vao; }
        GLuint getPositionVbo() const { return positionVbo; }
        GLuint getTexCoordVbo() const { return texCoordVbo; }
        GLuint getColorVbo() const { return colorVbo; }
        GLuint getIbo() const { return ibo; }

        GLuint getTextureId() const;

        const Font* getFont() const { return &props.font; }
        std::vector<TextSpan> getTextSpans() const { return spans; }
        int getNumVisibleChars() const { return numVisibleChars; }
        float getX() const { return x; }
        float getY() const { return y; }
        float getScale() const { return props.scale; }

    private:
        GLuint vao;
        GLuint positionVbo;
        GLuint texCoordVbo;
        GLuint colorVbo;
        GLuint ibo;

        std::vector<TextSpan> spans;
        TextProperties props;
        float x;
        float y;

        int numChars;
        int numVisibleChars;

        void refresh();
    };

}  // namespace Rival

#endif  // TEXT_RENDERABLE_H
