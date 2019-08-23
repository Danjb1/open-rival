#ifndef RIVAL_H
#define RIVAL_H

#include <gl/glew.h>
#include <SDL.h>
#include <iostream>
#include <string>

#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "UnitRenderer.h"
#include "Window.h"

namespace Rival {

    class Rival {

    public:

        /**
         * Initialises the game.
         */
        void initialize();

        /**
         * Runs the game.
         */
        void start();

    private:

        const int windowWidth = 800;
        const int windowHeight = 600;
        const char* windowTitle = "Rival Realms";

        std::unique_ptr<Texture> texture;
        std::unique_ptr<Sprite> sprite;
        std::unique_ptr<Texture> paletteTexture;
        std::unique_ptr<Scene> scene;
        std::unique_ptr<UnitRenderer> unitRenderer;

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
        Texture loadTexture(const std::string filename) const;

        /**
         * Handles keyDown events.
         */
        void keyDown(const SDL_Keycode keyCode) const;

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
