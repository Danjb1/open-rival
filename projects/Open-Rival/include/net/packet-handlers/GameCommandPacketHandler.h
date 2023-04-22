#pragma once

#include "net/packet-handlers/PacketHandler.h"

namespace Rival {

class GameCommandPacketHandler : public PacketHandler
{
    void onPacketReceived(std::shared_ptr<const Packet> packet, State& state) override;
};

}  // namespace Rival
