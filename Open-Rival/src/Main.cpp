#include "pch.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <gl/glew.h>
#include <iostream>
#include <stdexcept>

#include "Application.h"
#include "Rival.h"
#include "ScenarioBuilder.h"
#include "ScenarioData.h"
#include "ScenarioReader.h"
#include "Shaders.h"
#include "Window.h"

void initSDL() {

    // This must be called before SDL_Init since we're not using SDL_main
    // as an entry point
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

void initGLEW() {

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK) {
        std::cerr << "Error initialising GLEW:"
                  << glewGetErrorString(glewError) << "\n";
        throw std::runtime_error("Failed to initialise GLEW");
    }
}

void initGL() {

    // Try to enable vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Unable to enable vsync! SDL Error: %s\n", SDL_GetError());
        //vsyncEnabled = false;
    }

    // Set clear color
    glClearColor(0.5f, 0.5f, 1.f, 1.f);

    // Enable back-face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Enable alpha blending
    // See also:
    // https://www.khronos.org/opengl/wiki/Transparency_Sorting#Alpha_test
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Rival::initialiseShaders();
}

/**
 * Entry point for the application.
 */
int main() {

    try {

        // Initialize SDL
        initSDL();

        // Create our Window
        Rival::Window window(800, 600, "Rival Realms");
        window.use();

        // Initialize OpenGL
        initGLEW();
        initGL();

        // Create our Application
        Rival::Application app(window);

        // Load some scenario
        Rival::ScenarioReader reader(Rival::Resources::mapsDir + "example.sco");
        Rival::ScenarioData scenarioData = reader.readScenario();
        Rival::Scenario scenario(
                scenarioData.hdr.mapWidth,
                scenarioData.hdr.mapHeight,
                scenarioData.hdr.wilderness);
        Rival::ScenarioBuilder scenarioBuilder(scenarioData);
        scenarioBuilder.build(scenario);

        // Create our initial state
        Rival::Rival rival(app.getWindow(), scenario);

        // Run the game!
        app.start(rival);

    } catch (const std::runtime_error& e) {
        std::cerr << "Unhandled error during initialization or gameplay\n";
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
