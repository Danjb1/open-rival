#pragma once

#include <glm/glm.hpp>

#include <string>

#include "gfx/Color.h"
#include "gfx/Font.h"

namespace Rival {

/**
 * A portion of text with a configurable color.
 */
struct TextSpan
{
    std::string text;
    Color color;
};

/**
 * Properties that control how some text should be rendered.
 */
struct TextProperties
{
    /**
     * Font to use when rendering the text.
     */
    const Font* font = nullptr;

    /**
     * Scale at which to render the text.
     *
     * This is a fraction of the default font size.
     */
    float scale = 1.0f;

    /**
     * Whether the text should have a shadow.
     */
    bool hasShadow = true;
};

/**
 * Class that allows colored text to be rendered as textured quads.
 *
 * This creates the VAO and all necessary buffers.
 */
class TextRenderable
{
public:
    /**
     * Constructs a TextRenderable to hold a single TextSpan.
     */
    TextRenderable(TextSpan span, TextProperties props, float x, float y);

    /**
     * Constructs a TextRenderable to hold the given TextSpans.
     */
    TextRenderable(std::vector<TextSpan> spans, TextProperties props, float x, float y);

    /**
     * Constructs a TextRenderable to hold strings up to a given length.
     */
    TextRenderable(int maxChars, TextProperties props, float x, float y);

    /** Deletes a TextRenderable. */
    ~TextRenderable();

    // Allow moving but prevent copying and move-assignment
    TextRenderable(const TextRenderable& other) = delete;
    TextRenderable(TextRenderable&& other) noexcept;
    TextRenderable& operator=(const TextRenderable& other) = delete;
    TextRenderable& operator=(TextRenderable&& other) = delete;

    GLuint getVao() const
    {
        return vao;
    }

    GLuint getPositionVbo() const
    {
        return positionVbo;
    }

    GLuint getTexCoordVbo() const
    {
        return texCoordVbo;
    }

    GLuint getColorVbo() const
    {
        return colorVbo;
    }

    GLuint getIbo() const
    {
        return ibo;
    }

    GLuint getTextureId() const;

    const Font* getFont() const
    {
        return props.font;
    }

    std::vector<TextSpan> getTextSpans() const
    {
        return spans;
    }

    int getNumVisibleChars() const
    {
        return numVisibleChars;
    }

    float getX() const
    {
        return x;
    }

    float getY() const
    {
        return y;
    }

    float getScale() const
    {
        return props.scale;
    }

    int getNumLayers() const;

    void setTextSpan(TextSpan newSpan);
    void setTextSpans(std::vector<TextSpan> newSpans);

private:
    void countChars();
    void init();

public:
    static constexpr int numVertexDimensions = 3;    // x, y, z
    static constexpr int numTexCoordDimensions = 2;  // u, v
    static constexpr int numColorDimensions = 3;     // r, g, b
    static constexpr int numVerticesPerChar = 4;

    static constexpr int numLayersWithoutShadow = 1;
    static constexpr int numLayersWithShadow = 2;
    static constexpr int shadowLayerIndex = 0;
    static constexpr float shadowOffsetX = 0.f;
    static constexpr float shadowOffsetY = 1.f;

    static const Color defaultColor;
    static const Color highlightColor;

    /*
     * 6 indices are required to render a quad using GL_TRIANGLES:
     *  - First triangle: 0-1-2
     *  - Second triangle: 2-3-0
     */
    static constexpr int numIndicesPerChar = 6;

    mutable bool dirty = true;

private:
    GLuint vao = 0;
    GLuint positionVbo = 0;
    GLuint texCoordVbo = 0;
    GLuint colorVbo = 0;
    GLuint ibo = 0;

    std::vector<TextSpan> spans;
    TextProperties props;
    float x = 0;
    float y = 0;

    int numChars = 0;
    int numVisibleChars = 0;
};

}  // namespace Rival
