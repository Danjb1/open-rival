#include "pch.h"
#include "Rival.h"

#include <gl\glew.h>

#include "Image.h"
#include "Shaders.h"

bool initialised = false;

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

/**
 * Entry point for the application.
 */
int main(int argc, char* args[]) {

    Rival::Rival rival = Rival::Rival();

    try {
        rival.initialise();
    } catch (std::runtime_error e) {
        std::cerr << "Failed to initialise" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        rival.start();
    } catch (std::runtime_error e) {
        std::cerr << "Error during gameplay" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

namespace Rival {

    void Rival::initialise() {
        initSDL();
        window = createWindow();
        window->use();
        initGLEW();
        initGL();
        loadTextures();
    }

    void Rival::initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialise!" << std::endl;
            throw new std::runtime_error(SDL_GetError());
        }
    }

    std::unique_ptr<Window> Rival::createWindow() {

        // Set window attributes
        // Use OpenGL 3.1 core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
        window.create();

        return std::make_unique<Window>(window);
    }

    void Rival::initGLEW() {

        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();

        if (glewError != GLEW_OK) {
            std::cerr << "Error initialising GLEW:" << glewGetErrorString(glewError) << std::endl;
            throw new std::runtime_error("Failed to initialise GLEW");
        }
    }

    void Rival::initGL() {

        // Enable vsync
        if (SDL_GL_SetSwapInterval(1) < 0) {
            printf("Unable to enable vsync! SDL Error: %s\n", SDL_GetError());
        }

        // Set clear color
        glClearColor(0.5f, 0.5f, 1.f, 1.f);

        // Enable texturing
        glEnable(GL_TEXTURE_2D);

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initialiseShaders();
    }

    void Rival::loadTextures() {

        // Load texture
        Image sprite = loadImage("res\\textures\\knight.tga");
        std::cout << "Sprite is " << sprite.getWidth() << " x " << sprite.getHeight();

        // Generate texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sprite.getWidth(), sprite.getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, sprite.getData().get());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, NULL);

        // Check for error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("Error loading texture from %p pixels! %s\n", sprite.getData().get(), gluErrorString(error));
            throw new std::runtime_error("Failed to load texture");
        }

        // Create palette texture
        unsigned char *palette = new unsigned char[PALETTE_SIZE * PALETTE_CHANNELS];

        for (int i = 0; i < PALETTE_SIZE; i++) {
            int start = i * PALETTE_CHANNELS;
            uint32_t col = PALETTE[i];
            // RGBA
            palette[start] = (col & 0xff000000) >> 24;
            palette[start + 1] = (col & 0x00ff0000) >> 16;
            palette[start + 2] = (col & 0x0000ff00) >> 8;
            palette[start + 3] = col & 0x000000ff;
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
    }

    void Rival::start() {

        // Event handler
        SDL_Event e;

        // Run our game loop until the application is exited
        bool exiting = false;
        while (!exiting) {

            // Handle events on queue
            while (SDL_PollEvent(&e) != 0) {

                // User requests exiting
                if (e.type == SDL_QUIT) {
                    exiting = true;
                } else if (e.type == SDL_KEYDOWN) {
                    keyDown(e.key.keysym.sym);
                }
            }

            update();
            render();
            window->swapBuffers();
        }

        // Free resources and exit SDL
        exit();
    }

    void Rival::update() {

        if (initialised) {
            return;
        }

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

        initialised = true;
    }

    void Rival::render() {

        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader
        glUseProgram(textureShader.programId);

        // Use textures
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, textureID);
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, paletteTextureID);

        // Prepare
        glEnableVertexAttribArray(textureShader.vertexAttribLocation);
        glEnableVertexAttribArray(textureShader.texCoordAttribLocation);
        glUniform1i(textureShader.texUnitUniformLocation, 0);
        glUniform1i(textureShader.paletteTexUnitUniformLocation, 1);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        glVertexAttribPointer(textureShader.vertexAttribLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
        glBindBuffer(GL_ARRAY_BUFFER, gTexCoordVBO);
        glVertexAttribPointer(textureShader.texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);

        // Render
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

        // Clean up
        glDisableVertexAttribArray(textureShader.vertexAttribLocation);
        glDisableVertexAttribArray(textureShader.texCoordAttribLocation);
        glBindTexture(GL_TEXTURE_2D, NULL);
        glUseProgram(NULL);
    }

    void Rival::exit() {

        // Destroy window
        window->destroy();

        // Quit SDL
        SDL_Quit();
    }

    void Rival::keyDown(SDL_Keycode keyCode) {
        switch (keyCode) {
        case SDLK_UP:
            std::cout << "UP\n";
            break;

        case SDLK_DOWN:
            std::cout << "DOWN\n";
            break;

        case SDLK_LEFT:
            std::cout << "LEFT\n";
            break;

        case SDLK_RIGHT:
            std::cout << "RIGHT\n";
            break;

        default:
            break;
        }
    }

}
