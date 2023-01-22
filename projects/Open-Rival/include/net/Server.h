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

/**
 * Relay server that forwards received packets to every other client.
 */
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
    Socket serverSocket;
    ServerState state = ServerState::Lobby;
    std::thread acceptThread;

    int maxPlayers;
    int nextPlayerId = 0;
    std::unordered_map<int, std::unique_ptr<Connection>> connectedPlayers;
};

}  // namespace Rival
