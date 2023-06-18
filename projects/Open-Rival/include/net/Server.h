#pragma once

#include <cstdint>
#include <memory>
#include <thread>
#include <unordered_map>

#include "net/Connection.h"

namespace Rival {

class Socket;

/**
 * Relay server that forwards received packets to every other client.
 */
class Server : public ConnectionListener
{
public:
    Server(std::uint16_t port, int maxClients);
    virtual ~Server();

    // Begin ConnectionListener override
    void onPacketReceived(Connection& connection, std::shared_ptr<const Packet> packet) override;
    // End ConnectionListener override

    /** Starts accepting connections in a separate thread. */
    void start();

private:
    void acceptThreadLoop();
    int requestClientId();

private:
    Socket serverSocket;
    std::thread acceptThread;

    int maxClients;
    int nextClientId = 0;
    std::unordered_map<int, std::unique_ptr<Connection>> connectedClients;
};

}  // namespace Rival
