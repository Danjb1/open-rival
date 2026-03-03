#pragma once

#include "gfx/opengl/GLWrapper.h"

#include <memory>

#include "gfx/Image.h"
#include "gfx/Renderer.h"
#include "gfx/Texture.h"
#include "gfx/opengl/renderer/GameRenderer.h"

namespace Rival {

class Window;

/** OpenGL rendering implementation. */
class GLRenderer : public Renderer
{
public:
    GLRenderer(Window* window);

    void renderLobby(int delta) override;

    void onEnterGame(GameState* gameState) override;
    void renderGame(int delta) override;

    std::shared_ptr<const Texture> loadTexture(
            const std::string& filename, const TextureProperties& props = {}) override;
    std::shared_ptr<const Texture> createTexture(const Image& img, const TextureProperties& props = {}) override;

private:
    static void initGLEW();
    static void initGL();

    Window* window;
    std::unique_ptr<GameRenderer> gameRenderer;
};
}  // namespace Rival
