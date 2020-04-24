#ifndef SHADERS_H
#define SHADERS_H

#include <gl/glew.h>

namespace Rival {

    ///////////////////////////////////////////////////////////////////////////
    // IndexedTextureShader:
    // Renders a texture using a model-view matrix and an accompanying palette
    // texture for colour lookups.
    ///////////////////////////////////////////////////////////////////////////

    class IndexedTextureShader {
    public:

        GLuint programId;

        // Vertex shader uniform locations
        GLint viewProjMatrixUniformLocation;

        // Vertex shader attribute indices
        GLint vertexAttribIndex = 0;
        GLint texCoordAttribIndex = 1;

        // Vertex shader attribute locations
        GLint vertexAttribLocation;
        GLint texCoordAttribLocation;

        // Fragment shader uniform locations
        GLint texUnitUniformLocation;
        GLint paletteTexUnitUniformLocation;

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

        // Vertex shader attribute indices
        GLint vertexAttribIndex = 0;
        GLint texCoordAttribIndex = 1;

        // Vertex shader attribute locations
        GLint vertexAttribLocation;
        GLint texCoordAttribLocation;

        // Fragment shader uniform locations
        GLint texUnitUniformLocation;

        static void init();

        bool isValid() const;
    };

    extern ScreenShader screenShader;

    ///////////////////////////////////////////////////////////////////////////
    // Generic methods
    ///////////////////////////////////////////////////////////////////////////

    void initialiseShaders();

    GLuint createShader(const char* vertShader, const char* fragShader);

}

#endif // SHADERS_H
