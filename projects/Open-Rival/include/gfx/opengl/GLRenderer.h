#pragma once

#include "gfx/opengl/GLWrapper.h"

#include <memory>

#include "gfx/Image.h"
#include "gfx/Renderer.h"
#include "gfx/Texture.h"
#include "gfx/opengl/renderer/CursorRenderer.h"
#include "gfx/opengl/renderer/GameRenderer.h"
#include "gfx/opengl/renderer/LobbyRenderer.h"

namespace Rival {

class Input;
class Window;

/** OpenGL rendering implementation.
 * This also hosts the ImGui renderer backend, so an ImGui context must exist for the lifetime of this renderer. */
class GLRenderer : public Renderer
{
public:
    GLRenderer(Window* window, const Input& input);
    ~GLRenderer();

    void beginImGuiFrame() override;

    void onEnterLobby(LobbyState* lobby) override;
    void renderLobby(const FrameTiming& frameTiming) override;

    void onEnterGame(GameState* game) override;
    void renderGame(const FrameTiming& frameTiming) override;

    std::shared_ptr<const Texture> loadTexture(
            const std::string& filename, const TextureProperties& props = {}) override;
    std::shared_ptr<const Texture> createTexture(const Image& img, const TextureProperties& props = {}) override;

private:
    static void initGLEW();
    static void initGL();

    void initCursorRenderer(const TextureStore& textureStore);
    static void renderImGui();

    Window* window;
    const Input& input;
    std::unique_ptr<LobbyRenderer> lobbyRenderer;
    std::unique_ptr<GameRenderer> gameRenderer;

    /** Renders the cursor on top of everything else, regardless of the current State.
     * This is created on demand since the cursor textures are not available until resources have been loaded. */
    std::unique_ptr<CursorRenderer> cursorRenderer;
};
}  // namespace Rival
