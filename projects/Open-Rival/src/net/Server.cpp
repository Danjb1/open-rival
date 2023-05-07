#include "pch.h"

#include "net/Server.h"

#include <cstddef>  // std::size_t
#include <iostream>
#include <utility>  // std::make_pair, std::move

#include "net/Socket.h"

namespace Rival {

Server::Server(int port, int maxClients)
    : serverSocket(Socket::createServer(port))
    , maxClients(maxClients)
    , state(ServerState::Lobby)
{
}

Server::~Server()
{
    // Close connections
    for (auto& entry : connectedClients)
    {
        entry.second->close();
    }

    // Kill server
    serverSocket.close();
    acceptThread.join();
}

void Server::onPacketReceived(Connection& connection, std::shared_ptr<const Packet> packet)
{
    // Forward packets to all other connections
    for (const auto& entry : connectedClients)
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
    while (serverSocket.isOpen() && connectedClients.size() < static_cast<std::size_t>(maxClients))
    {
        Socket newPlayer = serverSocket.accept();
        if (newPlayer.isOpen())
        {
            int clientId = requestClientId();
            std::cout << "Client " << std::to_string(clientId) << " connected to server\n";

            auto newPlayerConnection = std::make_unique<Connection>(std::move(newPlayer), nullptr, clientId, this);
            connectedClients.insert(std::make_pair(clientId, std::move(newPlayerConnection)));
        }
    }
}

int Server::requestClientId()
{
    // The server just hands out client IDs incrementally.
    // Note that these are NOT the same as player IDs - they are just a way to uniquely identify a connection.
    // The server has no knowledge of player IDs since they are given out by the host inside of packets.
    int clientId = nextClientId;
    ++nextClientId;
    return clientId;
}

}  // namespace Rival
