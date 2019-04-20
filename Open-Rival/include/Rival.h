#ifndef RIVAL_H
#define RIVAL_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

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
         * Frees media and shuts down SDL.
         */
        void exit();

    private:

        const int SCREEN_WIDTH = 800;
        const int SCREEN_HEIGHT = 600;

        // The window we'll be rendering to
        SDL_Window* window = NULL;

        // The window renderer
        SDL_Renderer* renderer = NULL;

        // Current displayed texture
        SDL_Texture* titleTexture = NULL;

        /**
         * Starts up SDL and creates the window.
         */
        bool initSDL();

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
