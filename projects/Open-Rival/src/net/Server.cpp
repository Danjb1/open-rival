#include "pch.h"

#include "net/Server.h"

namespace Rival {

Server::Server(int port, int maxPlayers)
    : serverSocket(Socket::createServer(port))
    , maxPlayers(maxPlayers)
{
}

Server::~Server()
{
    // Close connections
    for (auto& entry : connectedPlayers)
    {
        entry.second.close();
    }

    // Kill server
    serverSocket.close();
    acceptThread.join();
}

void Server::start()
{
    acceptThread = std::thread(&Server::acceptThreadLoop, this);
}

void Server::acceptThreadLoop()
{
    while (!serverSocket.isClosed() && connectedPlayers.size() < static_cast<size_t>(maxPlayers))
    {
        std::shared_ptr<Socket> newPlayer = serverSocket.accept();
        if (newPlayer)
        {
            connectedPlayers.insert({ requestPlayerId(), newPlayer });
        }
    }
}

int Server::requestPlayerId()
{
    int playerId = nextPlayerId;
    ++nextPlayerId;
    return playerId;
}

}  // namespace Rival
