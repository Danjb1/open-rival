#pragma once

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

    const GLuint getId() const;

    const int getWidth() const;

    const int getHeight() const;

    static const Texture loadTexture(const std::string filename);

    static const Texture wrap(const Image img, TextureProperties props);

private:
    const GLuint id;

    const int width;

    const int height;
};

}  // namespace Rival
