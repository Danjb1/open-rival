#pragma once

#include <memory>

#include "gfx/TextureAtlas.h"

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

    /**
     * Constructs an AtlasRenderable.
     *
     * @param texAtlas
     * @param maxSprites The maximum number of Sprites that can be drawn.
     */
    AtlasRenderable(std::shared_ptr<const TextureAtlas> texAtlas, int maxSprites);
    ~AtlasRenderable();

    // Allow moving but prevent copying and move-assignment
    AtlasRenderable(const AtlasRenderable& other) = delete;
    AtlasRenderable(AtlasRenderable&& other) noexcept;
    AtlasRenderable& operator=(const AtlasRenderable& other) = delete;
    AtlasRenderable& operator=(AtlasRenderable&& other) = delete;

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
        return texAtlas->texture->getId();
    }

    GLenum getDrawMode() const
    {
        return drawMode;
    }

    int getIndicesPerSprite() const
    {
        return indicesPerSprite;
    }

public:
    std::shared_ptr<const TextureAtlas> texAtlas;

private:
    GLuint vao;
    GLuint positionVbo;
    GLuint texCoordVbo;
    GLuint ibo;

    GLenum drawMode;

    int indicesPerSprite;
};

}  // namespace Rival
