#ifndef RIVAL_H
#define RIVAL_H

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

        /**
         * Renders the current frame.
         */
        void render();

        /**
         * Frees media and shuts down SDL.
         */
        void exit();

    private:

        const int WINDOW_WIDTH = 800;
        const int WINDOW_HEIGHT = 600;
        const char* WINDOW_TITLE = "Rival Realms";

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
         * Creates the Window.
         */
        std::unique_ptr<Window> createWindow();

        /**
         * Loads the title image.
         */
        bool loadTitleImage();

        /**
         * Loads an image as a texture
         */
        SDL_Texture* loadTexture(std::string path);

        /**
         * Handles keyDown events.
         */
        void keyDown(SDL_Keycode keyCode);

    };

}

#endif // RIVAL_H