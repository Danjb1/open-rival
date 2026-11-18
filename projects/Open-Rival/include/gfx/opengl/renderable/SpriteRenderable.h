#pragma once

#include "gfx/opengl/GLWrapper.h"
#include <glm/vec2.hpp>

#include <memory>
#include <vector>

namespace Rival {

class Spritesheet;

/** A sprite from a Spritesheet, stretched to fill a rectangle. */
struct SpriteImage
{
    glm::vec2 position;
    glm::vec2 size;
    int spriteIndex;
};

/**
 * Class that allows one or more Sprites to be rendered from a Spritesheet as textured quads.
 *
 * This creates the VAO and all necessary buffers.
 */
class SpriteRenderable
{
public:
    static constexpr int numVertexDimensions = 3;    // x, y, z
    static constexpr int numTexCoordDimensions = 2;  // u, v
    static constexpr int numVerticesPerSprite = 4;

    std::shared_ptr<const Spritesheet> spritesheet;

    /**
     * Constructs a SpriteRenderable.
     *
     * @param spritesheet
     * @param maxSprites The maximum number of Sprites that can be drawn.
     */
    SpriteRenderable(std::shared_ptr<const Spritesheet> spritesheet, int maxSprites);

    /** Deletes a SpriteRenderable. */
    ~SpriteRenderable();

    // Disable moving / copying
    SpriteRenderable(const SpriteRenderable& other) = delete;
    SpriteRenderable(SpriteRenderable&& other) = delete;
    SpriteRenderable& operator=(const SpriteRenderable& other) = delete;
    SpriteRenderable& operator=(SpriteRenderable&& other) = delete;

    /** Uploads the given sprites to the GPU, replacing any previous ones.
     * Any sprites beyond the maximum are ignored. */
    void setSprites(const std::vector<SpriteImage>& sprites);

    /** Binds the spritesheet texture and draws the sprites most recently passed to setSprites. */
    void draw() const;

    GLuint getVao() const;
    GLuint getPositionVbo() const;
    GLuint getTexCoordVbo() const;
    GLuint getIbo() const;

    GLuint getTextureId() const;

    GLenum getDrawMode() const;

    int getIndicesPerSprite() const;

private:
    GLuint vao;
    GLuint positionVbo;
    GLuint texCoordVbo;
    GLuint ibo;

    GLenum drawMode;

    int indicesPerSprite;
    int maxSprites;
    int numSprites = 0;
};

}  // namespace Rival
