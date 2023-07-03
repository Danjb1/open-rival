#pragma once

#include "gfx/GlewWrapper.h"

namespace Rival { namespace GLUtils {

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

enum class PackAlignment : GLint
{
    BYTES_1 = 1,
    BYTES_2 = 2,
    BYTES_4 = 4,
    BYTES_8 = 8
};

class PixelStore
{
public:
    PixelStore(PackAlignment alignment);
    ~PixelStore();
};

}}  // namespace Rival::GLUtils
