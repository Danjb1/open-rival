#pragma once

#include <memory>
#include <vector>

#include "net/Packet.h"

namespace Rival {

/**
 * Deserializes packets from raw data.
 */
class PacketFactory
{
public:
    std::shared_ptr<Packet> deserialize(const std::vector<char>& buffer);
};

}  // namespace Rival
