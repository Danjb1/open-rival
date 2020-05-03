#ifndef RIVAL_H
#define RIVAL_H

#include <SDL.h>
#include <iostream>
#include <memory>

#include "BuildingRenderer.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "FramebufferRenderer.h"
#include "MapBorderRenderer.h"
#include "MousePicker.h"
#include "Resources.h"
#include "Scenario.h"
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

        // Framebuffer size, in pixels.
        // This is our canvas; we can never render more pixels than this.
        // We divide by 2 because our tiles overlap (see RenderUtils).
        static const int framebufferWidth =
                RenderUtils::tileWidthPx * RenderUtils::maxTilesX / 2;
        static const int framebufferHeight =
                RenderUtils::tileHeightPx * RenderUtils::maxTilesY / 2;

        // Resources
        std::unique_ptr<Resources> res;

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
        std::shared_ptr<Scenario> scenario;

        // Renderers
        std::unique_ptr<BuildingRenderer> buildingRenderer;
        std::unique_ptr<FramebufferRenderer> gameFboRenderer;
        std::unique_ptr<TileRenderer> tileRenderer;
        std::unique_ptr<MapBorderRenderer> mapBorderRenderer;
        std::unique_ptr<UnitRenderer> unitRenderer;

        /**
         * Window used to display the game.
         */
        std::unique_ptr<Window> window;
        bool vsyncEnabled = true;

        /**
         * Object used to find what's under the mouse.
         */
        std::unique_ptr<MousePicker> mousePicker;

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
        void renderGame(int viewportWidth, int viewportHeight);
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
