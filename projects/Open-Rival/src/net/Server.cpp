#include "pch.h"

#include "net/Server.h"

#include <utility>  // std::make_pair, std::move

#include "net/Socket.h"

namespace Rival {

Server::Server(int port, int maxPlayers)
    : serverSocket(Socket::createServer(port))
    , maxPlayers(maxPlayers)
    , state(ServerState::Lobby)
{
}

Server::~Server()
{
    // Close connections
    for (auto& entry : connectedPlayers)
    {
        entry.second->close();
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
        Socket newPlayer = serverSocket.accept();
        if (newPlayer.isValid())
        {
            std::unique_ptr<Connection> newPlayerConnection =
                    std::make_unique<Connection>(std::move(newPlayer), nullptr);
            connectedPlayers.insert(std::make_pair(requestPlayerId(), std::move(newPlayerConnection)));
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
