#ifndef RIVAL_H
#define RIVAL_H

#include <gl/glew.h>
#include <SDL.h>
#include <iostream>
#include <string>

#include "Window.h"

namespace Rival {

    class Rival {

    public:

        /**
         * Initialises the game.
         */
        void initialise();

        /**
         * Runs the game.
         */
        void start();

    private:

        const int WINDOW_WIDTH = 800;
        const int WINDOW_HEIGHT = 600;
        const char* WINDOW_TITLE = "Rival Realms";

        bool initialised = false;

        GLuint textureId = 0;
        GLuint paletteTextureId = 0;

        GLuint gVBO = 0;
        GLuint gTexCoordVBO = 0;
        GLuint gIBO = 0;

        /**
         * Window used to display the game.
         */
        std::unique_ptr<Window> window;

        /**
         * The window renderer.
         */
        SDL_Renderer* renderer = nullptr;

        /**
         * Initialises SDL.
         */
        void initSDL() const;

        /**
         * Initialises GLEW.
         */
        void initGLEW() const;

        /**
         * Initialises OpenGL.
         */
        void initGL();

        /**
         * Loads the game's textures.
         */
        GLuint loadTexture(const std::string filename) const;

        /**
         * Creates a texture to hold the game's palette.
         */
        GLuint createPaletteTexture() const;

        /**
         * Creates the Window.
         */
        std::unique_ptr<Window> createWindow() const;

        /**
         * Handles keyDown events.
         */
        void keyDown(const SDL_Keycode keyCode) const;

        /**
         * Renders the current frame.
         */
        void render() const;

        /**
         * Updates the game.
         */
        void update();

        /**
         * Frees media and shuts down SDL.
         */
        void exit() const;

    };

}

#endif // RIVAL_H
