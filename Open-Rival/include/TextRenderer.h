#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <glm/glm.hpp>

#include "TextRenderable.h"

namespace Rival {

    class TextRenderer {

    public:
        void render(const TextRenderable& textRenderable) const;

    private:
        static const int numVertexDimensions = 3;    // x, y, z
        static const int numTexCoordDimensions = 2;  // u, v
        static const int numVerticesPerSprite = 4;

        GLuint vao;
        GLuint positionVbo;
        GLuint texCoordVbo;
        GLuint ibo;

        int indicesPerSprite;

        bool needsUpdate(const TextRenderable& textRenderable) const;
        void sendDataToGpu(const TextRenderable& textRenderable) const;
    };

}  // namespace Rival

#endif  // TEXT_RENDERER_H
