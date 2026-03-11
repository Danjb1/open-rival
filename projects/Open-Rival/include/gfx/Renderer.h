#pragma once

#include "gfx/Texture.h"

namespace Rival {

class GameState;
class LobbyState;

/** Abstract base class for a rendering subsystem.
 * The idea is that game logic should depend on this class rather than any specific library.
 * This is responsible for all rendering operations, but also texture creation, since this will typically require some
 * GPU storage. */
class Renderer
{
public:
    virtual ~Renderer() {}

    /** Called whenever we enter the LobbyState. */
    virtual void onEnterLobby(LobbyState* lobbyState) = 0;

    /** Renders the LobbyState. */
    virtual void renderLobby(int delta) = 0;

    /** Called whenever we enter the GameState. */
    virtual void onEnterGame(GameState* game) = 0;

    /** Renders the GameState. */
    virtual void renderGame(int delta) = 0;

    /** Loads a Texture from a file. */
    virtual std::shared_ptr<const Texture> loadTexture(
            const std::string& filename, const TextureProperties& props = {}) = 0;

    /** Creates a Texture from the given Image data. */
    virtual std::shared_ptr<const Texture> createTexture(const Image& img, const TextureProperties& props = {}) = 0;
};

}  // namespace Rival
