#pragma once

#include <vector>

namespace Rival {

/**
 * A message that can be transmitted over a network.
 */
class Packet
{
public:
    /** Serializes this packet to the given buffer. */
    virtual void serialize(std::vector<char>& buffer) const = 0;

    /** Populates this packet by deserializing the given buffer. */
    virtual void deserialize(const std::vector<char> buffer) = 0;
};

}  // namespace Rival
