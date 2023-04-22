#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

#include "net/PacketFactory.h"
#include "net/Socket.h"
#include "net/packets/Packet.h"

namespace Rival {

class Connection;

/**
 * Interface that provides a callback for when a connection receives a new packet.
 */
class ConnectionListener
{
public:
    virtual void onPacketReceived(Connection& connection, std::shared_ptr<const Packet> packet) = 0;
};

/**
 * Manages a connection to a socket and provides operations to read and write game-specific packets.
 */
class Connection
{
public:
    Connection(
            Socket destination, std::shared_ptr<PacketFactory> packetFactory, ConnectionListener* listener = nullptr);
    ~Connection();

    bool Connection::operator==(const Connection& other) const;
    bool Connection::operator!=(const Connection& other) const;

    /** Closes this connection. */
    void close() noexcept;

    bool isClosed() const;

    /** Sends a packet on this connection. */
    void send(const Packet& packet);

    /**
     * Gets all packets received since the last call to this method.
     *
     * If this Connection has a listener, the packets will be sent there instead.
     */
    std::vector<std::shared_ptr<const Packet>> getReceivedPackets();

private:
    void receiveThreadLoop();

private:
    /** Buffer size used for reading from the socket. Packets should not exceed this size. */
    static constexpr size_t bufferSize = 512;

    Socket socket;
    std::shared_ptr<PacketFactory> packetFactory;

    std::vector<char> sendBuffer;
    std::vector<char> recvBuffer;

    std::thread receiveThread;
    std::vector<std::shared_ptr<const Packet>> receivedPackets;
    std::mutex receivedPacketsMutex;

    ConnectionListener* listener;
};

}  // namespace Rival
