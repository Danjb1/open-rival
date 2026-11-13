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
#include "window/Window.h"

namespace Rival {

class Socket;
class State;
class Renderer;

/**
 * Manages the application as a whole.
 * Holds references to all of our subsystems, runs the game loop and switches between "states" (e.g. menu, game).
 */
class Application : public WindowListener
{
public:
    Application(json& cfg,
            Window* window,
            Input& input,
            Renderer* renderer,
            AudioSystem& audioSystem,
            Resources& res,
            FT_Library fontLib);

    /**
     * Runs the Application until the user exits.
     */
    void start(std::unique_ptr<State> state);

    // Begin WindowListener override
    void onExitRequested() override;
    void onFocusLost() override;
    void onKeyDownEvent(const KeyEvent& evt) override;
    void onKeyUpEvent(const KeyEvent& evt) override;
    void onMouseDownEvent(const MouseButtonEvent& evt) override;
    void onMouseUpEvent(const MouseButtonEvent& evt) override;
    void onMouseMoveEvent(const MouseMoveEvent& evt) override;
    void onMouseWheelEvent(const MouseWheelEvent& evt) override;
    // End WindowListener override

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

    const Input& getInput() const
    {
        return input;
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

    /** Builds the ImGui UI for the current frame. */
    void buildImGui();

private:
    bool exiting { false };

    /** Whether to show the ImGui window (toggled with the backtick key). */
    bool isImGuiVisible { false };

    // Subsystems and resources
    json& cfg;
    Window* window;
    Input& input;
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
