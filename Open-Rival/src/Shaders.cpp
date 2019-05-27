#include "pch.h"
#include "Shaders.h"

#include "ShaderUtils.h"

namespace Rival {

    TextureShader textureShader;

    void initialiseShaders() {

        GLuint programId = createShader(
                "res\\shaders\\texture.vert",
                "res\\shaders\\texture.frag");

        textureShader = TextureShader();
        textureShader.programId = programId;
        textureShader.vertexAttribLocation = glGetAttribLocation(programId, "in_vertex");
        textureShader.texCoordAttribLocation = glGetAttribLocation(programId, "in_tex_coord");
        textureShader.texUnitUniformLocation = glGetUniformLocation(programId, "tex");
        textureShader.paletteTexUnitUniformLocation = glGetUniformLocation(programId, "palette");

        if (!textureShader.isValid()) {
            throw new std::runtime_error("Failed to create texture shader");
        }
    }

    GLuint createShader(const char* vertShader, const char* fragShader) {

        GLuint programId = glCreateProgram();

        // Create vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Read / set vertex source
        std::string vertexShaderSource = readShaderSource(vertShader);
        const char* vertexShaderSource2 = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);

        // Compile vertex source
        glCompileShader(vertexShader);

        // Check vertex shader for errors
        GLint vShaderCompiled = GL_FALSE;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
        if (vShaderCompiled != GL_TRUE) {
            printf("Unable to compile vertex shader %d!\n", vertexShader);
            printShaderLog(vertexShader);
            return 0;
        }

        // Attach vertex shader to program
        glAttachShader(programId, vertexShader);

        // Create fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Read / set fragment source
        std::string fragmentShaderSource = readShaderSource(fragShader);
        const char* fragmentShaderSource2 = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);

        // Compile fragment source
        glCompileShader(fragmentShader);

        // Check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
        if (fShaderCompiled != GL_TRUE) {
            printf("Unable to compile fragment shader %d!\n", fragmentShader);
            printShaderLog(fragmentShader);
            return 0;
        }

        // Attach fragment shader to program
        glAttachShader(programId, fragmentShader);

        // Link program
        glLinkProgram(programId);

        // Check for errors
        GLint programSuccess = GL_TRUE;
        glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
        if (programSuccess != GL_TRUE) {
            printf("Error linking program %d!\n", programId);
            printProgramLog(programId);
            return 0;
        }

        return programId;
    }

    bool TextureShader::isValid() {

        if (programId == 0) {
            printf("Could not generate program ID\n");
            return false;
        }

        if (vertexAttribLocation == -1) {
            printf("Could not locate vertex attribute\n");
            return false;
        }

        if (texCoordAttribLocation == -1) {
            printf("Could not locate tex co-ord attribute\n");
            return false;
        }

        if (texUnitUniformLocation == -1) {
            printf("Could not locate tex unit uniform\n");
            return false;
        }

        if (paletteTexUnitUniformLocation == -1) {
            printf("Could not locate palette tex unit uniform\n");
            return false;
        }

        return true;
    }

}
