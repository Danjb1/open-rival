#pragma once

#include <memory>
#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

/**
 * Packet sent by the host when starting the game.
 */
class StartGamePacket : public Packet
{
public:
    StartGamePacket();

    static std::shared_ptr<StartGamePacket> deserialize(const std::vector<char> buffer);
};

}  // namespace Rival
