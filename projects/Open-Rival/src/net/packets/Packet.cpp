#include "pch.h"

#include "net/packets/Packet.h"

namespace Rival {

void Packet::serialize(std::vector<char>& buffer) const
{
    // Later, we may need to ensure a certain endianness for cross-platform compatibility.
    // See: https://stackoverflow.com/questions/544928/reading-integer-size-bytes-from-a-char-array
    std::memcpy(buffer.data(), &type, sizeof(type));
}

}  // namespace Rival
