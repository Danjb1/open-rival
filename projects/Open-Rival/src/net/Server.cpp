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

void Server::onPacketReceived(Connection& connection, std::shared_ptr<const Packet> packet)
{
    for (const auto& entry : connectedPlayers)
    {
        const std::unique_ptr<Connection>& otherConnection = entry.second;
        if (connection == *otherConnection)
        {
            // Don't send packets back to the sender
            continue;
        }

        otherConnection->send(*packet);
    }
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
            auto newPlayerConnection = std::make_unique<Connection>(std::move(newPlayer), nullptr, this);
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
