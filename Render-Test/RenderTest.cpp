#include "RenderTest.h"

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
GLint gPaletteTextureUnitLocation;
GLuint textureID;
GLuint paletteTextureID;
GLuint gVBO = 0;
GLuint gTexCoordVBO = 0;
GLuint gIBO = 0;

// Number of colours in the palette
const int PALETTE_SIZE = 256;

// Number of channels per colour of the palette
const int PALETTE_CHANNELS = 4;

// The game's colour palette
const uint32_t PALETTE[PALETTE_SIZE] = {
    0x000000ff, 0xccb78fff, 0xa4a494ff, 0x8c846cff,	0x9c845cff, 0x9c7c54ff, 0x94744cff, 0x8c7454ff,
    0x846c54ff, 0x7b6747ff, 0x74644cff, 0x6c6454ff,	0xeacf09ff, 0xf0a705ff, 0xfe7f31ff, 0xfe5027ff,
    0xd10404ff, 0x9d1a1aff, 0x645c4cff, 0x6c5c44ff,	0x64543cff, 0x5c543cff, 0x545444ff, 0x4c5444ff,
    0x4c4c3cff, 0x544c3cff, 0x544c34ff, 0x5c4c34ff,	0x644c2cff, 0x64542cff, 0x6c5434ff, 0x745c34ff,
    0x7c542cff, 0x84542cff, 0x8c5c2cff, 0x8c5424ff,	0x9c6434ff, 0xa46c3cff, 0xb4743cff, 0xbc742cff,
    0xc47c34ff, 0xbc844cff, 0xbc8c54ff, 0xb48454ff,	0xac7c4cff, 0xcc8c4cff, 0xe88f37ff, 0xf49c54ff,
    0xcc7414ff, 0xdc6c04ff, 0xbc640cff, 0xac5c0cff,	0xac6c2cff, 0x8c6c44ff, 0x846444ff, 0x7c5c44ff,
    0x6c4c2cff, 0x644424ff, 0x5c442cff, 0x54442cff,	0x4c3c2cff, 0x443c2cff, 0x3c3c2cff, 0x3c3c34ff,
    0x343c34ff, 0x2c3434ff, 0x34342cff, 0x3d2e2eff,	0x3c2c24ff, 0x3c3424ff, 0x443424ff, 0x4c341cff,
    0x5c3c24ff, 0x643c1cff, 0x5c5a20ff, 0x444424ff,	0x444434ff, 0x24342cff, 0x242c2cff, 0x2c2424ff,
    0x2c2c1cff, 0x34241cff, 0x1c1c1cff, 0x14140cff,	0x0c0c0cff, 0x060605ff, 0x707522ff, 0x849324ff,
    0x94ac24ff, 0xa7c921ff, 0xb4dc24ff, 0xd4fc2cff,	0x041404ff, 0x0c1c04ff, 0x101104ff, 0x142c04ff,
    0x1c3404ff, 0x143414ff, 0x143c14ff, 0x144414ff,	0x144c14ff, 0x145414ff, 0x145c14ff, 0x0c4c0cff,
    0x0c440cff, 0x0c3c0cff, 0x04540cff, 0x2c4c0cff,	0x2c440cff, 0x344c0cff, 0x34540cff, 0x44640cff,
    0x4c740cff, 0x547c14ff, 0x548414ff, 0x5c9414ff,	0x649c14ff, 0x6cb41cff, 0x7cd41cff, 0x7c8444ff,
    0x6c6c3cff, 0x8c0000ff, 0x5b2b10ff, 0x201911ff,	0x2c1c14ff, 0x361f07ff, 0x3f2910ff, 0x463415ff,
    0xfbee9aff, 0xeecc81ff, 0xd3c7a5ff, 0xcfcab4ff,	0xc9baa3ff, 0xb7b099ff, 0xaca48cff, 0x9a9897ff,
    0x88949bff, 0x8c8c8cff, 0x7d848aff, 0x76716dff,	0x747c65ff, 0x766a57ff, 0x81693aff, 0x946c2cff,
    0x978872ff, 0x9f947aff, 0xbe9861ff, 0xeab456ff,	0xfc641cff, 0x666b72ff, 0xa4a4a4ff, 0xb4b4b4ff,
    0xb9bdc1ff, 0xccd0cfff, 0xe4d4c4ff, 0xc0321bff,	0x7d4e22ff, 0x605434ff, 0x595856ff, 0x2c1c2cff,
    0xff4155ff, 0xc7000eff, 0x88000dff, 0x68000aff,	0x500008ff, 0x380006ff, 0x44c1fcff, 0x0680c1ff,
    0x045b84ff, 0x044564ff, 0x04354cff, 0x042534ff,	0x41ff96ff, 0x00c760ff, 0x00883eff, 0x006830ff,
    0x005024ff, 0x003819ff, 0xffdd41ff, 0xc7aa00ff,	0x887100ff, 0x685600ff, 0x504200ff, 0x382e00ff,
    0xff8841ff, 0xc75100ff, 0x883400ff, 0x682800ff,	0x501e00ff, 0x381500ff, 0xff41c1ff, 0xc7007fff,
    0x88005aff, 0x680045ff, 0x500036ff, 0x380026ff,	0x4241ffff, 0x0700c7ff, 0x010088ff, 0x010068ff,
    0x000050ff, 0x000038ff, 0xa2a2a2ff, 0x616161ff,	0x3e3e3eff, 0x2b2b2bff, 0x1c1c1cff, 0x0b0b0bff,
    0xffffffff, 0xb1b1b1ff, 0x808080ff, 0x626262ff,	0x484848ff, 0x252525ff, 0x843c0cff, 0xd46414ff,
    0xfc9424ff, 0xfca424ff, 0xfcc434ff, 0xffe432ff,	0xfcfc3cff, 0xfcf46cff, 0xfcfc9cff, 0xe4c44cff,
    0x050583ff, 0x06066eff, 0x0202c4ff, 0x0202a0ff,	0x196c97ff, 0x115585ff, 0x10516dff, 0x1c80dcff,
    0x1770acff, 0x2476d1ff, 0x255698ff, 0x134572ff,	0x57350cff, 0x3e280dff, 0x31230cff, 0x040c3cff,
    0x0c1c64ff, 0x2c3cacff, 0x0c4cccff, 0x3c4cecff,	0x4c5ce4ff, 0x5c6cd4ff, 0x844cc4ff, 0x5414f4ff,
    0x1c84e4ff, 0x3474a4ff, 0x1c741cff, 0x1c9c1cff,	0x34d434ff, 0x44fc44ff, 0xfca4acff, 0xffffff00
};

class Image {

private:

    int width;
    int height;
    unsigned char *data;

public:

    Image(int width, int height) {
        this->width = width;
        this->height = height;
        this->data = new unsigned char[width * height];
    }

    Image(int width, int height, unsigned char *data) {
        this->width = width;
        this->height = height;
        this->data = data;
    }

    ~Image() {
        free(this->data);
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    unsigned char* getData() {
        return data;
    }

};

/**
 * Loads an image from a file.
 *
 * This is basically the reverse of `write_image` from image-extractor,
 * skipping all the data we don't care about.
 */
Image loadImage(std::string filename) {
    std::cout << "Loading: " << filename << "\n";

    std::ifstream ifs(filename, std::ios::binary | std::ios::in);
    if (!ifs) {
        throw std::runtime_error("Failed to load image!");
    }

    // Read sprite dimensions
    ifs.seekg(12);
    int width = ifs.get() | (ifs.get() << 8);
    int height = ifs.get() | (ifs.get() << 8);

    // Read pixel data
    unsigned char *data = new unsigned char[width * height];
    ifs.seekg(1042);
    ifs.read((char*)data, width * height);

    return Image(width, height, data);
}

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
    Image sprite = loadImage("res\\textures\\knight.tga");
    std::cout << "Sprite is " << sprite.getWidth() << " x " << sprite.getHeight();

    // Generate texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sprite.getWidth(), sprite.getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, sprite.getData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, NULL);

    // Check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error loading texture from %p pixels! %s\n", sprite.getData(), gluErrorString(error));
        return false;
    }

    // Create palette texture
    unsigned char *palette = new unsigned char[PALETTE_SIZE * PALETTE_CHANNELS];

    for (int i = 0; i < PALETTE_SIZE; i++) {
        int start = i * PALETTE_CHANNELS;
        uint32_t col = PALETTE[i];
        // RGBA
        palette[start]     = (col & 0xff000000) >> 24;
        palette[start + 1] = (col & 0x00ff0000) >> 16;
        palette[start + 2] = (col & 0x0000ff00) >> 8;
        palette[start + 3] =  col & 0x000000ff;
    }

    glGenTextures(1, &paletteTextureID);
    glBindTexture(GL_TEXTURE_2D, paletteTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, palette);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, NULL);

    delete palette;

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
    gPaletteTextureUnitLocation = glGetUniformLocation(gProgramID, "palette");
    if (gPaletteTextureUnitLocation == -1) {
        printf("palette is not a valid glsl program variable!\n");
    }

    // Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    // Use textures
    glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, paletteTextureID);

    // Prepare
    glEnableVertexAttribArray(gVertexPos2DLocation);
    glEnableVertexAttribArray(gVertexTexCoordLocation);
    glUniform1i(gTextureUnitLocation, 0);
    glUniform1i(gPaletteTextureUnitLocation, 1);
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

    // Delete textures
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    if (paletteTextureID != 0) {
        glDeleteTextures(1, &paletteTextureID);
        paletteTextureID = 0;
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

