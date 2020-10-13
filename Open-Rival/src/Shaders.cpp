#include "pch.h"
#include "Shaders.h"

#include <stdexcept>

#include "ShaderUtils.h"

namespace Rival {

    ///////////////////////////////////////////////////////////////////////////
    // GameWorldShader
    ///////////////////////////////////////////////////////////////////////////

    GameWorldShader gameWorldShader;

    void GameWorldShader::init() {

        GLuint programId = createShader(
                "res\\shaders\\gameWorld.vert",
                "res\\shaders\\gameWorld.frag");

        gameWorldShader = GameWorldShader();
        gameWorldShader.programId = programId;
        gameWorldShader.viewProjMatrixUniformLocation =
                glGetUniformLocation(programId, "view_proj_matrix");
        gameWorldShader.vertexAttribLocation =
                glGetAttribLocation(programId, "in_vertex");
        gameWorldShader.texCoordAttribLocation =
                glGetAttribLocation(programId, "in_tex_coord");
        gameWorldShader.texUnitUniformLocation =
                glGetUniformLocation(programId, "tex");
        gameWorldShader.paletteTexUnitUniformLocation =
                glGetUniformLocation(programId, "palette");

        if (!gameWorldShader.isValid()) {
            throw std::runtime_error("Failed to create GameWorldShader");
        }
    }

    bool GameWorldShader::isValid() const {

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

    ///////////////////////////////////////////////////////////////////////////
    // ScreenShader
    ///////////////////////////////////////////////////////////////////////////

    ScreenShader screenShader;

    void ScreenShader::init() {

        GLuint programId = createShader(
                "res\\shaders\\screen.vert",
                "res\\shaders\\screen.frag");

        screenShader = ScreenShader();
        screenShader.programId = programId;
        screenShader.vertexAttribLocation =
                glGetAttribLocation(programId, "in_vertex");
        screenShader.texCoordAttribLocation =
                glGetAttribLocation(programId, "in_tex_coord");
        screenShader.texUnitUniformLocation =
                glGetUniformLocation(programId, "tex");

        if (!screenShader.isValid()) {
            throw std::runtime_error("Failed to create ScreenShader");
        }
    }

    bool ScreenShader::isValid() const {

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

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    // MenuShader
    ///////////////////////////////////////////////////////////////////////////

    MenuShader menuShader;

    void MenuShader::init() {

        GLuint programId = createShader(
                "res\\shaders\\menu.vert",
                "res\\shaders\\menu.frag");

        menuShader = MenuShader();
        menuShader.programId = programId;
        menuShader.vertexAttribLocation =
                glGetAttribLocation(programId, "in_vertex");
        menuShader.texCoordAttribLocation =
                glGetAttribLocation(programId, "in_tex_coord");
        menuShader.texUnitUniformLocation =
                glGetUniformLocation(programId, "tex");
        menuShader.paletteTexUnitUniformLocation =
                glGetUniformLocation(programId, "palette");

        if (!menuShader.isValid()) {
            throw std::runtime_error("Failed to create MenuShader");
        }
    }

    bool MenuShader::isValid() const {

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

    ///////////////////////////////////////////////////////////////////////////
    // Generic methods
    ///////////////////////////////////////////////////////////////////////////

    void initialiseShaders() {
        GameWorldShader::init();
        ScreenShader::init();
        MenuShader::init();
    }

    GLuint createShader(const char* vertShader, const char* fragShader) {

        GLuint programId = glCreateProgram();

        // Create vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Read / set vertex source
        std::string vertexShaderSource =
                ShaderUtils::readShaderSource(vertShader);
        const char* vertexShaderSource2 = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderSource2, nullptr);

        // Compile vertex source
        glCompileShader(vertexShader);

        // Check vertex shader for errors
        GLint vShaderCompiled = GL_FALSE;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
        if (vShaderCompiled != GL_TRUE) {
            printf("Unable to compile vertex shader %d!\n", vertexShader);
            ShaderUtils::printShaderLog(vertexShader);
            return 0;
        }

        // Attach vertex shader to program
        glAttachShader(programId, vertexShader);

        // Create fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Read / set fragment source
        std::string fragmentShaderSource =
                ShaderUtils::readShaderSource(fragShader);
        const char* fragmentShaderSource2 = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSource2, nullptr);

        // Compile fragment source
        glCompileShader(fragmentShader);

        // Check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
        if (fShaderCompiled != GL_TRUE) {
            printf("Unable to compile fragment shader %d!\n", fragmentShader);
            ShaderUtils::printShaderLog(fragmentShader);
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
            ShaderUtils::printProgramLog(programId);
            return 0;
        }

        return programId;
    }

}  // namespace Rival
