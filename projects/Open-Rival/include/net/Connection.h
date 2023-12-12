#pragma once

#include <cstddef>  // std::size_t
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
    Connection(Socket destination,
            std::shared_ptr<PacketFactory> packetFactory,
            int remotePlayerId = -1,
            ConnectionListener* listener = nullptr);
    ~Connection();

    bool operator==(const Connection& other) const;
    bool operator!=(const Connection& other) const;

    /** Closes this connection. */
    void close() noexcept;

    bool isOpen() const;

    /** Sends a packet on this connection. */
    void send(const Packet& packet);

    /**
     * Gets all packets received since the last call to this method.
     *
     * If this Connection has a listener, the packets will be sent there instead.
     */
    std::vector<std::shared_ptr<const Packet>> getReceivedPackets();

    /**
     * Returns some packets to the connection, so that they will be retrieved in the next call to getReceivedPackets.
     */
    void returnPackets(std::vector<std::shared_ptr<const Packet>>& packetsToReturn);

private:
    void receiveThreadLoop();
    bool readFromSocket(std::size_t numBytes);

private:
    /** Maximum buffer size when receiving data. Packets should never exceed this size. */
    static constexpr std::size_t maxBufferSize = 512;

    Socket socket;
    std::shared_ptr<PacketFactory> packetFactory;

    std::vector<char> sendBuffer;
    std::vector<char> recvBuffer;

    std::thread receiveThread;
    std::vector<std::shared_ptr<const Packet>> receivedPackets;
    std::mutex receivedPacketsMutex;

    ConnectionListener* listener;

    /** Client ID of the player at the remote end of the connection.
     * On clients this will always be -1 since the server has no client ID. */
    int remoteClientId = -1;
};

}  // namespace Rival
