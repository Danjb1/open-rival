#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>  // std::move

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

    std::optional<Connection>& getConnection()
    {
        return connection;
    }

    /** Starts a server and connects to it. */
    void startServer(std::uint16_t port);

    /** Connects to a server. */
    void connectToServer(const std::string& address, std::uint16_t port);

private:
    void makeNextStateActive();

private:
    bool exiting { false };

    ApplicationContext& context;
    std::unique_ptr<State> state;
    std::unique_ptr<State> nextState;

    std::optional<Server> server;
    std::optional<Connection> connection;
    std::shared_ptr<PacketFactory> packetFactory;

    float gameSpeed = 1.f;
};

}  // namespace Rival
