#pragma once

#include <cstdint>
#include <vector>

namespace Rival {

enum class PacketType : std::uint16_t
{
    Invalid,
    AcceptPlayer,
    GameCommand
};

/**
 * A message that can be transmitted over a network.
 */
class Packet
{
public:
    virtual ~Packet() {}

    /** Serializes this packet to the given buffer. */
    virtual void serialize(std::vector<char>& buffer) const;

    PacketType getType() const
    {
        return type;
    }

private:
    PacketType type = PacketType::Invalid;
};

}  // namespace Rival
