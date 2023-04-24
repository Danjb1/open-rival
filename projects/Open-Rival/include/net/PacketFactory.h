#pragma once

#include <memory>
#include <vector>

#include "commands/GameCommandFactory.h"
#include "net/packets/Packet.h"

namespace Rival {

/**
 * Deserializes packets from raw data.
 */
class PacketFactory
{
public:
    std::shared_ptr<Packet> deserialize(const std::vector<char>& buffer) const;

private:
    std::shared_ptr<Packet> deserializeFromType(const std::vector<char>& buffer, PacketType type) const;

private:
    GameCommandFactory gameCommandFactory;
};

}  // namespace Rival
