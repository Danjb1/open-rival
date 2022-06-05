#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <GL\glew.h>

namespace Rival { namespace GLUtils {

    enum class PackAlignment : GLint
    {
        BYTES_1 = 1,
        BYTES_2 = 2,
        BYTES_4 = 4,
        BYTES_8 = 8
    };

    class PixelStore {
    public:
        PixelStore(PackAlignment alignment);
        ~PixelStore();
    };

}}  // namespace Rival::GLUtils

#endif  // GL_UTILS_H
