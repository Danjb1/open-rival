#ifndef RIVAL_H
#define RIVAL_H

#include <gl/glew.h>
#include <SDL.h>
#include <iostream>
#include <string>

#include "Camera.h"
#include "Framebuffer.h"
#include "Scenario.h"
#include "Sprite.h"
#include "Texture.h"
#include "TileRenderer.h"
#include "UnitRenderer.h"
#include "Window.h"

namespace Rival {

    class Rival {

    public:

        static const int windowWidth = 800;
        static const int windowHeight = 600;

        /**
         * Initialises the game.
         */
        void initialize();

        /**
         * Runs the game.
         */
        void start();

    private:

        const char* windowTitle = "Rival Realms";

        // Directories
        const std::string mapsDir = "res\\maps\\";
        const std::string txDir = "res\\textures\\";

        // Textures
        const int numTextures = 94;
        const int txIndexUnits = 0;
        const int txIndexTiles = 50;
        std::unique_ptr<std::vector<Texture>> textures =
                std::make_unique<std::vector<Texture>>();
        std::unique_ptr<Texture> paletteTexture;

        // Sprites
        std::unique_ptr<std::map<Unit::Type, Sprite>> unitSprites =
                std::make_unique<std::map<Unit::Type, Sprite>>();
        std::unique_ptr<std::map<int, Sprite>> tileSprites =
                std::make_unique<std::map<int, Sprite>>();

        // Camera
        std::unique_ptr<Camera> camera;

        // Scenario
        std::unique_ptr<Scenario> scenario;

        // Renderers
        std::unique_ptr<TileRenderer> tileRenderer;
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
         * Initializes SDL.
         */
        void initSDL() const;

        /**
         * Initializes GLEW.
         */
        void initGLEW() const;

        /**
         * Initializes OpenGL.
         */
        void initGL();

        /**
         * Loads the game's Textures.
         */
        void loadTextures();

        /**
         * Initializes the Sprites used by Units.
         */
        void initUnitSprites();

        /**
         * Initializes the Sprite for a Unit type.
         */
        void initUnitSprite(Unit::Type type, int txIndex);

        /**
         * Initializes the Sprites used by tiles.
         */
        void initTileSprites();

        /**
         * Initializes the Sprite for a tile type.
         */
        void initTileSprite(int type, int txIndex);

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
