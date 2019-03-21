#include "RenderTest.h"

#define STBI_ONLY_TGA
#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "ShaderUtils.h"
#include "stb_image.h"

namespace rival {

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// OpenGL context
SDL_GLContext gContext;

// Shader
GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLint gVertexTexCoordLocation = -1;
GLint gTextureUnitLocation = -1;
GLuint textureID;
GLuint gVBO = 0;
GLuint gTexCoordVBO = 0;
GLuint gIBO = 0;

bool init() {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create window
    gWindow = SDL_CreateWindow(
        "Render Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (gWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Create context
    gContext = SDL_GL_CreateContext(gWindow);
    if (gContext == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
    }

    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }

    // Initialize OpenGL
    if (!initGL()) {
        printf("Unable to initialize OpenGL!\n");
        return false;
    }

    // Load texture
    int width, height, n;
    stbi_set_flip_vertically_on_load(1);
    stbi_set_skip_tga_palette_lookup(1);
    unsigned char *data = stbi_load("res\\textures\\knight.tga", &width, &height, &n, 0);
    if (data == NULL) {
        printf("Failed to load image!\n");
        printf("Error: %s\n", stbi_failure_reason());
        return false;
    }

    // Generate texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, NULL);

    // Check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error loading texture from %p pixels! %s\n", data, gluErrorString(error));
        return false;
    }

    // Free image data (no longer needed)
    stbi_image_free(data);

    return true;
}

bool initGL() {

    // Generate program
    gProgramID = glCreateProgram();

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Read / set vertex source
    std::string vertexShaderSource = rival::readShaderSource("res\\shaders\\texture.vert");
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
        return false;
    }

    // Attach vertex shader to program
    glAttachShader(gProgramID, vertexShader);

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read / set fragment source
    std::string fragmentShaderSource = readShaderSource("res\\shaders\\texture.frag");
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
        return false;
    }

    // Attach fragment shader to program
    glAttachShader(gProgramID, fragmentShader);

    // Link program
    glLinkProgram(gProgramID);

    // Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        printf("Error linking program %d!\n", gProgramID);
        printProgramLog(gProgramID);
        return false;
    }

    // Get vertex attribute locations
    gVertexPos2DLocation = glGetAttribLocation(gProgramID, "in_vertex");
    if (gVertexPos2DLocation == -1) {
        printf("in_vertex is not a valid glsl program variable!\n");
        return false;
    }
    gVertexTexCoordLocation = glGetAttribLocation(gProgramID, "in_tex_coord");
    if (gVertexTexCoordLocation == -1) {
        printf("in_tex_coord is not a valid glsl program variable!\n");
        return false;
    }

    // Get uniform locations
    gTextureUnitLocation = glGetUniformLocation(gProgramID, "tex");
    if (gTextureUnitLocation == -1) {
        printf("tex is not a valid glsl program variable!\n");
    }

    // Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    //Enable texturing
    glEnable(GL_TEXTURE_2D);

    // VBO data
    GLfloat vertexData[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
        -0.5f,  0.5f
    };
    GLfloat texCoordData[] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1
    };

    // IBO data
    GLuint indexData[] = { 0, 1, 2, 3 };

    // Create vertex pos VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    // Create tex coord VBO
    glGenBuffers(1, &gTexCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gTexCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), texCoordData, GL_STATIC_DRAW);

    // Create IBO
    glGenBuffers(1, &gIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    return true;
}

void render() {

    glClear(GL_COLOR_BUFFER_BIT);

    // Use shader
    glUseProgram(gProgramID);

    // Use texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Prepare
    glEnableVertexAttribArray(gVertexPos2DLocation);
    glEnableVertexAttribArray(gVertexTexCoordLocation);
    glUniform1i(gTextureUnitLocation, 0);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
    glBindBuffer(GL_ARRAY_BUFFER, gTexCoordVBO);
    glVertexAttribPointer(gVertexTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

    // Bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);

    // Render
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

    // Clean up
    glDisableVertexAttribArray(gVertexPos2DLocation);
    glDisableVertexAttribArray(gVertexTexCoordLocation);
    glBindTexture(GL_TEXTURE_2D, NULL);
    glUseProgram(NULL);
}

void close() {

    // Delete texture
    if (textureID != 0 ) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    // Deallocate program
    glDeleteProgram(gProgramID);

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

} // namespace rival

int main(int argc, char* args[]) {

    // Initialise SDL
    if (!rival::init()) {
        printf("Failed to initialize!\n");
        rival::close();
        return 1;
    }

    bool quit = false;

    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        rival::render();
        SDL_GL_SwapWindow(rival::gWindow);
    }

    rival::close();
    return 0;
}

