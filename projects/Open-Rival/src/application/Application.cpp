#include "application/Application.h"

#ifdef ENABLE_IMGUI
#include <imgui.h>
#endif

#include <algorithm>  // std::min
#include <chrono>
#include <memory>
#include <optional>
#include <utility>  // std::in_place, std::move

#include "application/State.h"
#include "game/PlayerState.h"
#include "net/Socket.h"
#include "utils/JsonUtils.h"
#include "utils/LogUtils.h"
#include "utils/TimeUtils.h"

namespace Rival {

Application::Application(json& cfg,
        Window* window,
        Input& input,
        Renderer* renderer,
        AudioSystem& audioSystem,
        Resources& res,
        FT_Library fontLib)
    : cfg(cfg)
    , window(window)
    , input(input)
    , renderer(renderer)
    , audioSystem(audioSystem)
    , res(res)
    , fontLib(fontLib)
{
    window->setListener(this);
}

void Application::start(std::unique_ptr<State> initialState)
{
    setState(std::move(initialState));

    const bool vsyncEnabled = window->isVsyncEnabled();

    TimeUtils::PrecisionTimer timer;

    // Game loop
    TimeUtils::TimePoint nextUpdateDue = TimeUtils::Clock::now();
    std::optional<TimeUtils::TimePoint> lastRenderTime;
    while (!exiting)
    {
        // Switch to the next State, if set
        if (nextState.get())
        {
            makeNextStateActive();
        }

        // Determine when this frame began.
        // If we are running behind, this will be a long time after
        // nextUpdateDue, so we will have to update the logic multiple
        // times to catch up. If we are ahead of schedule, this will be
        // BEFORE nextUpdateDue, and we will need to wait a little longer.
        TimeUtils::TimePoint frameStartTime = TimeUtils::Clock::now();

        // Is the next update due?
        if (vsyncEnabled || nextUpdateDue <= frameStartTime)
        {
            // Handle events on the queue
            window->pollEvents();

            // We can change the game speed by simulating more or less
            // time in each tick. This should be done with caution as it
            // could break determinism, and it will certainly break
            // multiplayer.
            int logicDelta = static_cast<int>(TimeUtils::timeStepMs * gameSpeed);

            // Update the game logic as many times as necessary to keep it
            // up to date with the current time. The idea is to run the
            // logic at a fixed rate, regardless of the game's framerate.
            //
            // For example:
            //  - For a 30Hz monitor, this will run twice per render.
            //  - For a 60Hz monitor, this will run once per render.
            //  - For a 120Hz monitor, this will run every other render.
            //
            // If vsync is disabled, this should run once per render.
            while (nextUpdateDue <= frameStartTime)
            {
                state->update(logicDelta);
                nextUpdateDue += std::chrono::milliseconds(TimeUtils::timeStepMs);
            }

            // Calculate our timing information for rendering
            const TimeUtils::TimePoint now = TimeUtils::Clock::now();
            FrameTiming frameTiming;

            // Time since the last render; this is used to advance visual animations.
            // We clamp this so that animations don't jump too far ahead after a long stall.
            if (lastRenderTime)
            {
                const auto timeSinceLastRender =
                        std::min<TimeUtils::Clock::duration>(now - *lastRenderTime, TimeUtils::maxTimeSinceLastRender);
                frameTiming.timeSinceLastRender = std::chrono::duration<float>(timeSinceLastRender).count();
            }
            lastRenderTime = now;

            // Time since the game logic was last updated; this enables us to
            // accurately extrapolate unit positions between logical movements.
            const TimeUtils::TimePoint lastUpdate = nextUpdateDue - std::chrono::milliseconds(TimeUtils::timeStepMs);
            frameTiming.timeSinceLastUpdate = std::chrono::duration<float>(now - lastUpdate).count();

#ifdef ENABLE_IMGUI
            // Build this frame's ImGui UI. This must happen before rendering
            // the State, since the Renderer draws the resulting ImGui data.
            renderer->beginImGuiFrame();
            window->beginImGuiFrame();
            ImGui::NewFrame();
            buildImGui();
            ImGui::Render();
#endif

            // Render the game, once per iteration. With vsync enabled, this
            // will be called at the same frequency as the screen's refresh
            // rate. Otherwise, this will be called at a frequency that
            // matches our target FPS.
            state->render(frameTiming);

            // Update the window with our newly-rendered game.
            // If vsync is enabled, this will block execution until the
            // next swap interval.
            window->swapBuffers();
        }
        else
        {
            // Next update is not yet due (frameStartTime < nextUpdateDue),
            // so let's sleep (unless the next update is imminent!)
            auto sleepTime = std::chrono::duration<float>(nextUpdateDue - frameStartTime);
            if (sleepTime >= std::chrono::milliseconds(TimeUtils::minSleepTime))
            {
                timer.wait(sleepTime);
            }
        }
    }
}

void Application::onExitRequested()
{
    exiting = true;
}

void Application::onFocusLost()
{
    // We won't receive any key / button release events while unfocused
    input.releaseAll();
}

// Note that we always update the Input BEFORE forwarding events to the State,
// so that the State sees an up-to-date Input when handling the event.

void Application::onKeyDownEvent(const KeyEvent& evt)
{
    input.onKeyDown(evt);

#ifdef ENABLE_IMGUI
    if (evt.key == KeyCode::Backtick)
    {
        isImGuiVisible = !isImGuiVisible;
    }
#endif

    if (state)
    {
        state->keyDown(evt);
    }
}

void Application::onKeyUpEvent(const KeyEvent& evt)
{
    input.onKeyUp(evt);

    if (state)
    {
        state->keyUp(evt);
    }
}

void Application::onMouseDownEvent(const MouseButtonEvent& evt)
{
    input.onMouseDown(evt);

    if (state)
    {
        state->mouseDown(evt);
    }
}

void Application::onMouseUpEvent(const MouseButtonEvent& evt)
{
    input.onMouseUp(evt);

    if (state)
    {
        state->mouseUp(evt);
    }
}

void Application::onMouseMoveEvent(const MouseMoveEvent& evt)
{
    input.onMouseMoved(evt);
}

void Application::onMouseWheelEvent(const MouseWheelEvent& evt)
{
    if (state)
    {
        state->mouseWheelMoved(evt);
    }
}

#ifdef ENABLE_IMGUI
void Application::buildImGui()
{
    if (isImGuiVisible)
    {
        ImGui::ShowDemoWindow(&isImGuiVisible);
    }
}
#endif

void Application::makeNextStateActive()
{
    state = std::move(nextState);
    state->onLoad();
}

void Application::startServer(std::uint16_t port)
{
    LOG_INFO("Starting server on port {}", port);

    server.emplace(port, PlayerStore::maxPlayers);
    server->start();

    // Connect to the server ourselves!
    connectToServer("localhost", port);
}

void Application::connectToServer(const std::string& address, std::uint16_t port)
{
    LOG_INFO("Connecting to server on port {}", port);

    if (!packetFactory)
    {
        packetFactory = std::make_shared<PacketFactory>();
    }

    Socket clientSocket = Socket::createClient(address, port);
    connection.emplace(std::move(clientSocket), packetFactory);
}

}  // namespace Rival
