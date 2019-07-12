#include "pch.h"
#include "Rival.h"

#include <map>

#include "Image.h"
#include "Palette.h"
#include "Shaders.h"

namespace Rival {

    Rival::Rival() {}

    void Rival::initialize() {
        initSDL();
        window = std::make_unique<Window>(
                windowWidth, windowHeight, windowTitle);
        window->use();
        initGLEW();
        initGL();

        // Load Textures
        texture = std::make_unique<Texture>(
                loadTexture("res\\textures\\unit_human_knight.tga"));
        paletteTexture = std::make_unique<Texture>(
                createPaletteTexture());

        // Define Sprites
        sprite = std::make_unique<Sprite>(*texture, 128, 128);

        // Create the Scene
        scene = std::make_unique<Scene>();

        // Add a Unit
        std::unique_ptr<Unit> unit = std::make_unique<Unit>(*sprite.get());
        scene->addUnit(std::move(unit));

        // Create the UnitRenderer
        unitRenderer = std::make_unique<UnitRenderer>(*paletteTexture.get());
    }

    void Rival::initSDL() const {

        SDL_SetMainReady();

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialise!\n";
            throw std::runtime_error(SDL_GetError());
        }

        // Use OpenGL 3.1 core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);
    }

    void Rival::initGLEW() const {

        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();

        if (glewError != GLEW_OK) {
            std::cerr << "Error initialising GLEW:"
                    << glewGetErrorString(glewError) << "\n";
            throw std::runtime_error("Failed to initialise GLEW");
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

    Texture Rival::loadTexture(const std::string filename) const {

        // Load image data
        Image img = loadImage(filename);

        // Generate texture
        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                img.getWidth(), img.getHeight(),
                0, GL_RED, GL_UNSIGNED_BYTE, img.getData()->data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Check for error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("Error loading texture from %p pixels! %s\n",
                    img.getData(), gluErrorString(error));
            throw std::runtime_error("Failed to load texture");
        }

        return Texture(textureId, img.getWidth(), img.getHeight());
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
        std::map<int, std::unique_ptr<Unit>>& units = scene->getUnits();
        for (auto const& kv : units) {
            const std::unique_ptr<Unit>& unit = kv.second;
            unit->facing = (unit->facing + 1) % 8;
            unit->renderable.txIndex = unit->facing;
        }
    }

    void Rival::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        unitRenderer->render(scene->getUnits());
    }

    void Rival::exit() {

        // Destroy window
        window.reset();

        // Quit SDL
        SDL_Quit();
    }

    void Rival::keyDown(const SDL_Keycode keyCode) const {
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
