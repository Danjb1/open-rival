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
    Packet(PacketType type, int playerId);
    virtual ~Packet() {}

    /** Serializes this packet to the given buffer. */
    virtual void serialize(std::vector<char>& buffer) const;

    PacketType getType() const
    {
        return type;
    }

    int getPlayerId() const
    {
        return playerId;
    }

public:
    static constexpr size_t playerIdOffset = sizeof(PacketType);

protected:
    /** Offset of the first data after the packet header. */
    static constexpr size_t packetDataOffset = sizeof(PacketType) + sizeof(int) /* playerId */;

private:
    PacketType type;
    int playerId;
};

}  // namespace Rival
