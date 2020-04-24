#ifndef RIVAL_H
#define RIVAL_H

#include <SDL.h>
#include <iostream>
#include <string>

#include "Camera.h"
#include "Framebuffer.h"
#include "FramebufferRenderer.h"
#include "Scenario.h"
#include "Spritesheet.h"
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

        // Texture constants
        static const int numTextures = 94;
        static const int txIndexUnits = 0;
        static const int txIndexTiles = 50;

        // Framebuffer size, in pixels.
        // This is our canvas; we can never render more pixels than this.
        // We divide by 2 because our tiles overlap (see RenderUtils).
        static const int framebufferWidth =
                RenderUtils::tileWidthPx * RenderUtils::maxTilesX / 2;
        static const int framebufferHeight =
                RenderUtils::tileHeightPx * RenderUtils::maxTilesY / 2;

        // Loaded textures
        std::unique_ptr<std::vector<Texture>> textures =
                std::make_unique<std::vector<Texture>>();
        std::unique_ptr<Texture> paletteTexture;

        // Sprites
        std::unique_ptr<std::map<Unit::Type, Spritesheet>> unitSprites =
                std::make_unique<std::map<Unit::Type, Spritesheet>>();
        std::unique_ptr<std::map<int, Spritesheet>> tileSprites =
                std::make_unique<std::map<int, Spritesheet>>();

        // Camera
        std::unique_ptr<Camera> camera;

        /**
         * Framebuffer to which the visible region of the game is rendered at
         * a pixel-perfect scale.
         *
         * This ensures that there are no seams between tiles, and we can
         * perform any scaling when we render the framebuffer to the screen.
         */
        std::unique_ptr<Framebuffer> gameFbo;

        // Scenario
        std::unique_ptr<Scenario> scenario;

        // Renderers
        std::unique_ptr<FramebufferRenderer> gameFboRenderer;
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
         * Handles mouse wheel events.
         */
        void mouseWheelMoved(const SDL_MouseWheelEvent evt) const;

        /**
         * Renders the current frame.
         */
        void render();
        void renderGame();
        void renderFramebuffer();

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
