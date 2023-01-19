#pragma once

#include <memory>

#include "net/Connection.h"
#include "net/Server.h"

namespace Rival {

class ApplicationContext;
class Socket;
class State;

/**
 * Runs the game loop and switches between states.
 */
class Application
{
public:
    Application(ApplicationContext& context);

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

    ApplicationContext& getContext()
    {
        return context;
    }

    State& getState()
    {
        return *state;
    }

    /** Starts a server and connects to it. */
    void Application::startServer(int port);

private:
    void makeNextStateActive();

private:
    bool exiting { false };

    ApplicationContext& context;
    std::unique_ptr<State> state;
    std::unique_ptr<State> nextState;

    std::unique_ptr<Server> server;
    std::unique_ptr<Connection> connection;
};

}  // namespace Rival
