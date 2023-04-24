#pragma once

#include <memory>
#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

/**
 * Deserializes packets from raw data.
 */
class PacketFactory
{
public:
    std::shared_ptr<Packet> deserialize(const std::vector<char>& buffer);

private:
    std::shared_ptr<Packet> deserializeFromType(const std::vector<char>& buffer, PacketType type);
};

}  // namespace Rival
