#ifndef SHADERS_H
#define SHADERS_H

#include <gl/glew.h>

namespace Rival {

    class TextureShader {
    public:

        GLuint programId;

        // Vertex shader uniforms
        GLint viewProjMatrixUniformLocation;

        // Vertex shader attributes
        GLint vertexAttribLocation;
        GLint texCoordAttribLocation;

        // Fragment shader uniforms
        GLint texUnitUniformLocation;
        GLint paletteTexUnitUniformLocation;

        bool isValid() const;
    };

    extern TextureShader textureShader;

    void initialiseShaders();

    GLuint createShader(const char* vertShader, const char* fragShader);

}

#endif // SHADERS_H
