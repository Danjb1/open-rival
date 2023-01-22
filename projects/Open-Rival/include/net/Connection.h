#pragma once

#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "net/Packet.h"
#include "net/PacketFactory.h"
#include "net/Socket.h"

namespace Rival {

/**
 * Manages a connection to a socket and provides operations to read and write game-specific packets.
 */
class Connection
{
public:
    Connection(Socket destination, std::shared_ptr<PacketFactory> packetFactory);
    ~Connection();

    /** Closes this connection. */
    void close() noexcept;

    /** Sends a packet on this connection. */
    void send(const Packet& packet);

    /** Gets all packets received since the last call to this method. */
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
};

}  // namespace Rival
