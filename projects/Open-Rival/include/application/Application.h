#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>  // std::move

#include "application/Resources.h"
#include "audio/AudioSystem.h"
#include "net/Connection.h"
#include "net/Server.h"
#include "utils/JsonUtils.h"

namespace Rival {

class Socket;
class State;
class Renderer;
class Window;

/**
 * Manages the application as a whole.
 * Holds references to all of our subsystems, runs the game loop and switches between "states" (e.g. menu, game).
 */
class Application
{
public:
    Application(json& cfg,
            Window* window,
            Renderer* renderer,
            AudioSystem& audioSystem,
            Resources& res,
            FT_Library fontLib);

    /**
     * Runs the Application until the user exits.
     */
    void start(std::unique_ptr<State> state);

    void pollEvents();

    void requestExit()
    {
        exiting = true;
    }

    void setState(std::unique_ptr<State> newState)
    {
        nextState = std::move(newState);
    }

    State& getState()
    {
        return *state;
    }

    std::optional<Connection>& getConnection()
    {
        return connection;
    }

    /** Starts a server and connects to it. */
    void startServer(std::uint16_t port);

    /** Connects to a server. */
    void connectToServer(const std::string& address, std::uint16_t port);

    Window* getWindow() const
    {
        return window;
    }

    Renderer* getRenderer() const
    {
        return renderer;
    }

    json& getConfig()
    {
        return cfg;
    }

    const json& getConfig() const
    {
        return cfg;
    }

    AudioSystem& getAudioSystem()
    {
        return audioSystem;
    }

    Resources& getResources()
    {
        return res;
    }

    FT_Library getFontLibrary() const
    {
        return fontLib;
    }

private:
    void makeNextStateActive();

private:
    bool exiting { false };

    // Subsystems and resources
    json& cfg;
    Window* window;
    Renderer* renderer;
    AudioSystem& audioSystem;
    Resources& res;
    FT_Library fontLib;

    std::unique_ptr<State> state;
    std::unique_ptr<State> nextState;

    std::optional<Server> server;
    std::optional<Connection> connection;
    std::shared_ptr<PacketFactory> packetFactory;

    float gameSpeed = 1.f;
};

}  // namespace Rival
