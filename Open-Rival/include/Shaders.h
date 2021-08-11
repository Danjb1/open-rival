#ifndef SHADERS_H
#define SHADERS_H

#include <gl/glew.h>

namespace Rival {
namespace Shaders {

    // Vertex shader attribute indices.
    // We keep these common between all shaders so that our VAOs can be
    // rendered by any shader without requiring reconfiguration.
    static const GLint vertexAttribIndex = 0;
    static const GLint texCoordAttribIndex = 1;

    ///////////////////////////////////////////////////////////////////////////
    // IndexedTextureShader:
    // Renders a texture using a view-projection matrix and an accompanying
    // palette texture for colour lookups.
    ///////////////////////////////////////////////////////////////////////////

    class IndexedTextureShader {
    public:
        GLuint programId;

        // Vertex shader uniform locations
        GLint viewProjMatrixUniformLoc;

        // Vertex shader attribute locations
        GLint vertexAttribLoc;
        GLint texCoordAttribLoc;

        // Fragment shader uniform locations
        GLint texUnitUniformLoc;
        GLint paletteTexUnitUniformLoc;

        static void init();

        bool isValid() const;
    };

    extern IndexedTextureShader indexedTextureShader;

    ///////////////////////////////////////////////////////////////////////////
    // ScreenShader:
    // Just renders a texture without applying any transformations.
    ///////////////////////////////////////////////////////////////////////////

    class ScreenShader {
    public:
        GLuint programId;

        // Vertex shader attribute locations
        GLint vertexAttribLoc;
        GLint texCoordAttribLoc;

        // Fragment shader uniform locations
        GLint texUnitUniformLoc;

        static void init();

        bool isValid() const;
    };

    extern ScreenShader screenShader;

    ///////////////////////////////////////////////////////////////////////////
    // Generic methods
    ///////////////////////////////////////////////////////////////////////////

    void initializeShaders();

    GLuint createShader(const char* vertShader, const char* fragShader);

}}  // namespace Rival::Shaders

#endif  // SHADERS_H
