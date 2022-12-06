#pragma once

#include "TextureAtlas.h"

namespace Rival {

/**
 * Class that allows one or more images to be rendered from a TextureAtlas
 * as textured quads.
 *
 * This creates the VAO and all necessary buffers.
 */
class AtlasRenderable
{

public:
    static constexpr int numVertexDimensions = 3;    // x, y, z
    static constexpr int numTexCoordDimensions = 2;  // u, v
    static constexpr int numVerticesPerSprite = 4;

    const TextureAtlas& texAtlas;

    /**
     * Constructs an AtlasRenderable.
     *
     * @param texAtlas
     * @param maxSprites The maximum number of Sprites that can be drawn.
     */
    AtlasRenderable(const TextureAtlas& texAtlas, int maxSprites);

    // Disable moving / copying
    AtlasRenderable(const AtlasRenderable& other) = delete;
    AtlasRenderable(AtlasRenderable&& other) = delete;
    AtlasRenderable& operator=(const AtlasRenderable& other) = delete;
    AtlasRenderable& operator=(AtlasRenderable&& other) = delete;

    /** Deletes an AtlasRenderable. */
    ~AtlasRenderable();

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

    GLuint getIbo() const
    {
        return ibo;
    }

    GLuint getTextureId() const
    {
        return texAtlas.texture.getId();
    }

    GLenum getDrawMode() const
    {
        return drawMode;
    }

    int getIndicesPerSprite() const
    {
        return indicesPerSprite;
    }

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