#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "Texture.h"

namespace Rival {

    class TextRenderer {

    public:
        TextRenderer();
        void render(Texture& texture);

    private:
        static const int numVertexDimensions = 3;    // x, y, z
        static const int numTexCoordDimensions = 2;  // u, v
        static const int numVerticesPerSprite = 4;

        GLuint vao;
        GLuint positionVbo;
        GLuint texCoordVbo;
        GLuint ibo;

        int indicesPerSprite;
    };

}  // namespace Rival

#endif  // TEXT_RENDERER_H
