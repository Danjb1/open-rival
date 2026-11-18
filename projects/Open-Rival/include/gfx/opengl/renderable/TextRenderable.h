#pragma once

#include "gfx/opengl/GLWrapper.h"

#include <vector>

#include "gfx/Color.h"
#include "gfx/Text.h"

namespace Rival {

/**
 * Class that allows colored text to be rendered as textured quads.
 *
 * This creates the VAO and all necessary buffers.
 */
class TextRenderable
{
public:
    /** Constructs a TextRenderable with no text. */
    TextRenderable();

    /** Deletes a TextRenderable. */
    ~TextRenderable();

    // Allow moving but prevent copying and move-assignment
    TextRenderable(const TextRenderable& other) = delete;
    TextRenderable(TextRenderable&& other) noexcept;
    TextRenderable& operator=(const TextRenderable& other) = delete;
    TextRenderable& operator=(TextRenderable&& other) = delete;

    /** Uploads the given text to the GPU, replacing any previous text.
     * (x, y) is the start of the text's baseline. Any unsupported characters are skipped. */
    void setText(const std::vector<TextSpan>& spans, const TextProperties& newProps, float x, float y);

    /** Binds the font texture and draws the text most recently passed to setText. */
    void draw() const;

public:
    static const Color defaultColor;
    static const Color highlightColor;

private:
    int getNumLayers() const;
    bool isShadowLayer(int layer) const;

private:
    static constexpr int numVertexDimensions = 3;    // x, y, z
    static constexpr int numTexCoordDimensions = 2;  // u, v
    static constexpr int numColorDimensions = 3;     // r, g, b
    static constexpr int numVerticesPerChar = 4;

    /*
     * 6 indices are required to render a quad using GL_TRIANGLES:
     *  - First triangle: 0-1-2
     *  - Second triangle: 2-3-0
     */
    static constexpr int numIndicesPerChar = 6;

    static constexpr int numLayersWithoutShadow = 1;
    static constexpr int numLayersWithShadow = 2;
    static constexpr int shadowLayerIndex = 0;
    static constexpr float shadowOffsetX = 0.f;
    static constexpr float shadowOffsetY = 1.f;

    GLuint vao = 0;
    GLuint positionVbo = 0;
    GLuint texCoordVbo = 0;
    GLuint colorVbo = 0;
    GLuint ibo = 0;

    /** Properties passed to the last call to setText. */
    TextProperties props;

    /** Number of indices uploaded by the last call to setText. */
    int numIndices = 0;
};

}  // namespace Rival
