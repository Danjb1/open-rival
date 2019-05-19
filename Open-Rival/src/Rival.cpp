#include "pch.h"
#include "Rival.h"

#include <gl\glew.h>

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

        // Copied from RenderTest - needs refactoring
        /*

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
            return false;
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

        */
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

            render();
            window->swapBuffers();
        }

        // Free resources and exit SDL
        exit();
    }

    void Rival::render() {

        glClear(GL_COLOR_BUFFER_BIT);

        // Copied from RenderTest - needs refactoring
        /*

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
        */
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