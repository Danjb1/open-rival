#ifndef RIVAL_H
#define RIVAL_H

#include <SDL.h>
#include <iostream>
#include <memory>

#include "Application.h"
#include "BuildingRenderer.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "FramebufferRenderer.h"
#include "MapBorderRenderer.h"
#include "MousePicker.h"
#include "Rect.h"
#include "Scenario.h"
#include "State.h"
#include "TileRenderer.h"
#include "UnitRenderer.h"
#include "Window.h"

namespace Rival {

    class Rival : public State {

    public:
        Rival(Window& window, Scenario& scenario);

        // Inherited from State
        void initialize(Application* app);
        void keyDown(const SDL_Keycode keyCode) override;
        void mouseWheelMoved(const SDL_MouseWheelEvent evt) override;
        void render() override;
        void update() override;

    private:
        // Framebuffer size, in pixels.
        // This is our canvas; we can never render more pixels than this.
        static const int framebufferWidth;
        static const int framebufferHeight;

        Window& window;

        // Camera
        Camera camera;

        // Viewport:
        // the rectangle on the screen to which the game is rendered (pixels)
        Rect viewport;

        /**
         * Framebuffer to which the visible region of the game is rendered at
         * a pixel-perfect scale.
         *
         * This ensures that there are no seams between tiles, and we can
         * perform any scaling when we render the framebuffer to the screen.
         */
        std::unique_ptr<Framebuffer> gameFbo;

        // Scenario
        Scenario& scenario;

        // Renderers
        std::unique_ptr<BuildingRenderer> buildingRenderer;
        std::unique_ptr<FramebufferRenderer> gameFboRenderer;
        std::unique_ptr<TileRenderer> tileRenderer;
        std::unique_ptr<MapBorderRenderer> mapBorderRenderer;
        std::unique_ptr<UnitRenderer> unitRenderer;

        /**
         * Object used to find what's under the mouse.
         */
        std::unique_ptr<MousePicker> mousePicker;

        /**
         * The window renderer.
         */
        SDL_Renderer* renderer = nullptr;

        void renderGame(int viewportWidth, int viewportHeight);
        void renderFramebuffer(int srcWidth, int srcHeight);
    };

}  // namespace Rival

#endif  // RIVAL_H
