#include "gfx/opengl/GLUtils.h"

#include <stdexcept>

namespace Rival { namespace GLUtils {

PixelStore::PixelStore(PackAlignment alignment)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, static_cast<GLint>(alignment));
}

PixelStore::~PixelStore()
{
    // Reset to default
    glPixelStorei(GL_UNPACK_ALIGNMENT, static_cast<GLint>(PackAlignment::BYTES_4));
}

GLenum toGL(TextureFilterMode mode)
{
    switch (mode)
    {
    case TextureFilterMode::Nearest:
        return GL_NEAREST;
    case TextureFilterMode::Linear:
        return GL_LINEAR;
    }
    throw std::runtime_error("Unsupported filter mode");
}

GLenum toGL(TextureWrapMode mode)
{
    switch (mode)
    {
    case TextureWrapMode::Repeat:
        return GL_REPEAT;
    case TextureWrapMode::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case TextureWrapMode::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    }
    throw std::runtime_error("Unsupported wrap mode");
}

GLenum toGL(TexturePixelFormat format)
{
    switch (format)
    {
    case TexturePixelFormat::SingleByte:
        return GL_RED;
    case TexturePixelFormat::RGBA:
        return GL_RGBA;
    }
    throw std::runtime_error("Unsupported pixel format");
}

}}  // namespace Rival::GLUtils
