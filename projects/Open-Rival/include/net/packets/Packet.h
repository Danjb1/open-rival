#pragma once

#include <cstdint>
#include <vector>

namespace Rival {

enum class PacketType : std::uint8_t
{
    // N.B. Any packet types added here should also be added to PacketFactory!
    Invalid,
    RequestJoin,
    AcceptPlayer,
    RejectPlayer,
    LobbyWelcome,
    KickPlayer,
    StartGame,
    GameCommand
};

/**
 * A message that can be transmitted over a network.
 */
class Packet
{
public:
    Packet(PacketType type);
    virtual ~Packet() {}

    /** Serializes this packet to the given buffer. */
    virtual void serialize(std::vector<char>& buffer) const;

    /** Finalizes this packet before sending. */
    void finalize(std::vector<char>& buffer) const;

    PacketType getType() const
    {
        return type;
    }

    void setClientId(int newClientId)
    {
        clientId = newClientId;
    }

    int getClientId() const
    {
        return clientId;
    }

public:
    /** Size, in bytes, of the packet size sent before a packet. */
    static constexpr size_t sizeBytes = sizeof(int);

protected:
    /** Size of the packet header, in bytes, of a packet received from the relay server.
     * This excludes the packet size, which is read separately before the packet data. */
    static constexpr size_t relayedPacketHeaderSize = sizeof(PacketType) + sizeof(int) /* client ID */;

    /** Client ID of the sender, will be populated on all packets received from the relay server. */
    int clientId = -1;

private:
    PacketType type;
};

}  // namespace Rival
