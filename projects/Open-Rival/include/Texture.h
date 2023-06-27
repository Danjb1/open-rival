#pragma once

#define GLEW_STATIC
#include <gl/glew.h>

#include <string>

#include "Image.h"

namespace Rival {

struct TextureProperties
{
    /**
     * Minification filter.
     *
     * Generally, we have to stick to GL_NEAREST since we use indexed
     * textures, so interpolation between colors is impossible.
     */
    GLint minFilter = GL_NEAREST;

    /**
     * Magnification filter.
     *
     * Generally, we have to stick to GL_NEAREST since we use indexed
     * textures, so interpolation between colors is impossible.
     */
    GLint magFilter = GL_NEAREST;

    /**
     * Texture wrapping mode.
     */
    GLint wrapMode = GL_CLAMP_TO_EDGE;
};

class Texture
{
public:
    Texture(const GLuint id, int width, int height);
    ~Texture();

    // Allow moving but prevent copying and move-assignment
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) = delete;

    const GLuint getId() const;

    const int getWidth() const;

    const int getHeight() const;

    static std::shared_ptr<const Texture> loadTexture(const std::string filename);

    static std::shared_ptr<const Texture> wrap(const Image img, TextureProperties props);

private:
    GLuint id;
    int width;
    int height;
};

}  // namespace Rival
