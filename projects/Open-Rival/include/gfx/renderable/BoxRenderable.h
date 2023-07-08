#pragma once

#include "gfx/GlewWrapper.h"

namespace Rival {

/**
 * Class that allows one or more coloured quads to be rendered.
 *
 * This creates the VAO and all necessary buffers.
 */
class BoxRenderable
{
public:
    static constexpr int numVertexDimensions = 3;    // x, y, z
    static constexpr int numTexCoordDimensions = 2;  // u, v
    static constexpr int numVerticesPerSprite = 4;

    /**
     * Constructs a BoxRenderable.
     *
     * @param maxBoxes The maximum number of boxes that can be drawn.
     */
    BoxRenderable(int maxBoxes);

    /** Deletes a BoxRenderable. */
    ~BoxRenderable();

    // Disable moving / copying
    BoxRenderable(const BoxRenderable& other) = delete;
    BoxRenderable(BoxRenderable&& other) = delete;
    BoxRenderable& operator=(const BoxRenderable& other) = delete;
    BoxRenderable& operator=(BoxRenderable&& other) = delete;

    GLuint getVao() const;
    GLuint getPositionVbo() const;
    GLuint getTexCoordVbo() const;
    GLuint getIbo() const;

    GLenum getDrawMode() const;

    int getIndicesPerBox() const;

private:
    GLuint vao;
    GLuint positionVbo;
    GLuint texCoordVbo;
    GLuint ibo;

    GLenum drawMode;

    int indicesPerBox;
};

}  // namespace Rival
