#pragma once

#include "gfx/opengl/GLWrapper.h"
#include <glm/vec2.hpp>

#include <memory>
#include <string>
#include <vector>

#include "gfx/TextureAtlas.h"

namespace Rival {

/** An image from a TextureAtlas, stretched to fill a rectangle. */
struct AtlasImage
{
    glm::vec2 position;
    glm::vec2 size;
    std::string imageKey;
};

/**
 * Class that allows one or more images to be rendered from a TextureAtlas
 * as textured quads.
 *
 * This creates the VAO and all necessary buffers.
 */
class AtlasRenderable
{
public:
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

    /** Uploads the given images to the GPU, replacing any previous ones.
     * Any images beyond the maximum are ignored. */
    void setImages(const std::vector<AtlasImage>& images);

    /** Binds the atlas texture and draws the images most recently passed to setImages. */
    void draw() const;

public:
    std::shared_ptr<const TextureAtlas> texAtlas;

private:
    static constexpr int numVertexDimensions = 3;    // x, y, z
    static constexpr int numTexCoordDimensions = 2;  // u, v
    static constexpr int numVerticesPerSprite = 4;

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
