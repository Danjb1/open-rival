#ifndef FRAMEBUFFER_RENDERER_H
#define FRAMEBUFFER_RENDERER_H

#include "Framebuffer.h"

namespace Rival {

    /**
     * Class responsible for rendering a framebuffer to the screen.
     */
    class FramebufferRenderer {

    public:
        FramebufferRenderer(Framebuffer& fbo);

        /**
         * Renders a portion of this framebuffer's texture to the screen.
         *
         * To achieve pixel-perfect rendering, `srcWidth` and `srcHeight`
         * should be the exact same size as the viewport, so that no stretching
         * occurs.
         */
        void render(int srcWidth, int srcHeight);

    private:
        static const int numVertexDimensions = 2;    // x, y
        static const int numTexCoordDimensions = 2;  // u, v

        static const int numIndices = 4;

        Framebuffer& fbo;

        GLuint vao;
        GLuint positionVbo;
        GLuint texCoordVbo;
        GLuint ibo;
    };

}  // namespace Rival

#endif  // FRAMEBUFFER_RENDERER_H
