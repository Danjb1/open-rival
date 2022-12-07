#pragma once

#include <nlohmann/json.hpp>

#include <memory>

#include "AudioSystem.h"
#include "Resources.h"
#include "State.h"
#include "Window.h"

using json = nlohmann::json;

namespace Rival {

class Application
{
public:
    Application(Window& window, json& cfg);

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

    const Window& getWindow() const
    {
        return window;
    }

    AudioSystem& getAudioSystem()
    {
        return audioSystem;
    }

    Resources& getResources()
    {
        return res;
    }

    State& getState()
    {
        return *state;
    }

private:
    void makeNextStateActive();

public:
    bool vsyncEnabled;

private:
    /**
     * Minimum time that we will consider sleeping for.
     *
     * If the next frame is due sooner than this then we will just
     * busy-wait, to reduce the risk of oversleeping.
     */
    static constexpr int minSleepTime = 2;

    bool exiting { false };

    Window& window;
    json& cfg;
    AudioSystem audioSystem;
    Resources res;

    std::unique_ptr<State> state;
    std::unique_ptr<State> nextState;
};

}  // namespace Rival
