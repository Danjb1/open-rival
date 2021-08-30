#include "pch.h"
#include "GLUtils.h"

namespace Rival {
namespace GLUtils {

    PixelStore::PixelStore(PackAlignment alignment) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, static_cast<GLint>(alignment));
    }

    PixelStore::~PixelStore() {
        // Reset to default
        glPixelStorei(GL_UNPACK_ALIGNMENT,
                static_cast<GLint>(PackAlignment::BYTES_4));
    }

}}  // namespace Rival::GLUtils
