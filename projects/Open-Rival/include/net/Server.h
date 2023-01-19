#pragma once

#include <cstdint>
#include <memory>
#include <thread>
#include <unordered_map>

#include "net/Connection.h"

namespace Rival {

class Socket;

enum class ServerState : std::uint8_t
{
    Lobby,
    Game
};

class Server
{
public:
    Server(int port, int maxPlayers);
    ~Server();

    /** Starts accepting connections in a separate thread. */
    void start();

private:
    void acceptThreadLoop();
    int requestPlayerId();

private:
    std::unique_ptr<Socket> serverSocket;
    ServerState state = ServerState::Lobby;
    std::thread acceptThread;

    int maxPlayers;
    int nextPlayerId = 0;
    std::unordered_map<int, Connection> connectedPlayers;
};

}  // namespace Rival
