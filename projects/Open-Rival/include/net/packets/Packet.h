#pragma once

#include <cstdint>
#include <vector>

namespace Rival {

enum class PacketType : std::uint8_t
{
    Invalid,
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

    PacketType getType() const
    {
        return type;
    }

    void setPlayerId(int newPlayerId)
    {
        playerId = newPlayerId;
    }

    int getPlayerId() const
    {
        return playerId;
    }

protected:
    /** Size of the packet header, in bytes, of a packet received from the relay server. */
    static constexpr size_t relayedPacketHeaderSize = sizeof(PacketType) + sizeof(int) /* playerId */;

    /** Player ID of the sender, will be populated on all packets received from the relay server. */
    int playerId = -1;

private:
    PacketType type;
};

}  // namespace Rival
