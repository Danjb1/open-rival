#pragma once

#include <gl/glew.h>

namespace Rival {

class Spritesheet;

/**
 * Class that allows one or more Sprites to be rendered from a Spritesheet
 * as textured quads.
 *
 * This creates the VAO and all necessary buffers.
 */
class SpriteRenderable
{
public:
    static constexpr int numVertexDimensions = 3;    // x, y, z
    static constexpr int numTexCoordDimensions = 2;  // u, v
    static constexpr int numVerticesPerSprite = 4;

    const Spritesheet& spritesheet;

    /**
     * Constructs a SpriteRenderable.
     *
     * @param spritesheet
     * @param maxSprites The maximum number of Sprites that can be drawn.
     */
    SpriteRenderable(const Spritesheet& spritesheet, int maxSprites);

    /** Deletes a SpriteRenderable. */
    ~SpriteRenderable();

    // Disable moving / copying
    SpriteRenderable(const SpriteRenderable& other) = delete;
    SpriteRenderable(SpriteRenderable&& other) = delete;
    SpriteRenderable& operator=(const SpriteRenderable& other) = delete;
    SpriteRenderable& operator=(SpriteRenderable&& other) = delete;

    GLuint getVao() const;
    GLuint getPositionVbo() const;
    GLuint getTexCoordVbo() const;
    GLuint getIbo() const;

    GLuint getTextureId() const;

    GLenum getDrawMode() const;

    int getIndicesPerSprite() const;

private:
    /*
     * 4 indices are required to render a quad using GL_TRIANGLE_FAN:
     *     0------1
     *     | \    |
     *     |   \  |
     *     3----- 2
     */
    static constexpr int numIndicesForTriangleFan = 4;

    /*
     * 6 indices are required to render a quad using GL_TRIANGLES:
     *  - First triangle: 0-1-2
     *  - Second triangle: 2-3-0
     */
    static constexpr int numIndicesForTriangles = 6;

    GLuint vao;
    GLuint positionVbo;
    GLuint texCoordVbo;
    GLuint ibo;

    GLenum drawMode;

    int indicesPerSprite;
};

}  // namespace Rival