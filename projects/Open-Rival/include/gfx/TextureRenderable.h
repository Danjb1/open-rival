#pragma once

#define GLEW_STATIC
#include <gl/glew.h>

#include <memory>

namespace Rival {

class Texture;

/**
 * Class that allows an entire texture to be rendered.
 *
 * This creates the VAO and all necessary buffers.
 */
class TextureRenderable
{
public:
    static constexpr int numVertexDimensions = 3;    // x, y, z
    static constexpr int numTexCoordDimensions = 2;  // u, v
    static constexpr int numVertices = 4;

    TextureRenderable(std::shared_ptr<const Texture> texture);
    ~TextureRenderable();

    // Disable moving / copying
    TextureRenderable(const TextureRenderable& other) = delete;
    TextureRenderable(TextureRenderable&& other) = delete;
    TextureRenderable& operator=(const TextureRenderable& other) = delete;
    TextureRenderable& operator=(TextureRenderable&& other) = delete;

    GLuint getVao() const;
    GLuint getPositionVbo() const;
    GLuint getTexCoordVbo() const;
    GLuint getIbo() const;

    GLuint getTextureId() const;

private:
    std::shared_ptr<const Texture> texture;

    GLuint vao;
    GLuint positionVbo;
    GLuint texCoordVbo;
    GLuint ibo;
};

}  // namespace Rival
