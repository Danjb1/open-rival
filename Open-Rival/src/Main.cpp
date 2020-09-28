#include "pch.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <gl/glew.h>
#include <iostream>
#include <stdexcept>

#include "Application.h"
#include "Rival.h"
#include "Scenario.h"
#include "ScenarioBuilder.h"
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

    int exitCode = 0;

    try {

        // Initialize SDL
        initSDL();

        // Create our Window
        Rival::Window window(800, 600, "Rival Realms");

        // Initialize OpenGL
        initGLEW();
        initGL();

        // Create our Application
        Rival::Application app(window);

        // Load some scenario
        Rival::ScenarioReader reader(Rival::Resources::mapsDir + "example.sco");
        Rival::ScenarioBuilder scenarioBuilder(reader.readScenario());
        std::unique_ptr<Rival::Scenario> scenario = scenarioBuilder.build();

        // Create our initial state
        std::unique_ptr<Rival::State> initialState =
                std::make_unique<Rival::Rival>(app, std::move(scenario));

        // Run the game!
        app.start(std::move(initialState));

    } catch (const std::exception& e) {
        std::cerr << "Unhandled error during initialization or gameplay\n";
        std::cerr << e.what() << "\n";
        exitCode = 1;
    }

    SDL_Quit();

    return exitCode;
}
