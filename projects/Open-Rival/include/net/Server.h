#pragma once

#include <cstdint>
#include <memory>
#include <thread>
#include <unordered_map>

#include "net/Connection.h"
#include "net/PacketFactory.h"

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
    Server(int port, int maxPlayers, std::shared_ptr<PacketFactory> packetFactory);
    ~Server();

    /** Starts accepting connections in a separate thread. */
    void start();

private:
    void acceptThreadLoop();
    int requestPlayerId();

private:
    Socket serverSocket;
    std::shared_ptr<PacketFactory> packetFactory;
    ServerState state = ServerState::Lobby;
    std::thread acceptThread;

    int maxPlayers;
    int nextPlayerId = 0;
    std::unordered_map<int, std::unique_ptr<Connection>> connectedPlayers;
};

}  // namespace Rival
