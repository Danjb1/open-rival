#pragma once

#include <memory>
#include <optional>
#include <string>

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

    void pollNetwork();
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

    Connection& getConnection()
    {
        return *connection;
    }

    /** Starts a server and connects to it. */
    void startServer(int port);

    /** Connects to a server. */
    void connectToServer(const std::string& address, int port);

private:
    void makeNextStateActive();

private:
    bool exiting { false };

    ApplicationContext& context;
    std::unique_ptr<State> state;
    std::unique_ptr<State> nextState;

    std::optional<Server> server;
    std::optional<Connection> connection;
};

}  // namespace Rival
