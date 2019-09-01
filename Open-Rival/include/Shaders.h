#ifndef SHADERS_H
#define SHADERS_H

#include <gl/glew.h>

namespace Rival {

    class TextureShader {
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

        bool isValid() const;
    };

    extern TextureShader textureShader;

    void initialiseShaders();

    GLuint createShader(const char* vertShader, const char* fragShader);

}

#endif // SHADERS_H
