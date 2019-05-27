#ifndef RIVAL_H
#define RIVAL_H

#include <gl\glew.h>
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
        SDL_Renderer* renderer = NULL;

        /**
         * Initialises SDL.
         */
        void initSDL();

        /**
         * Initialises GLEW.
         */
        void initGLEW();

        /**
         * Initialises OpenGL.
         */
        void initGL();

        /**
         * Loads the game's textures.
         */
        GLuint loadTexture(std::string filename);

        /**
         * Creates a texture to hold the game's palette.
         */
        GLuint createPaletteTexture();

        /**
         * Creates the Window.
         */
        std::unique_ptr<Window> createWindow();

        /**
         * Handles keyDown events.
         */
        void keyDown(SDL_Keycode keyCode);

        /**
         * Renders the current frame.
         */
        void render();

        /**
         * Updates the game.
         */
        void update();

        /**
         * Frees media and shuts down SDL.
         */
        void exit();

    };

}

#endif // RIVAL_H
