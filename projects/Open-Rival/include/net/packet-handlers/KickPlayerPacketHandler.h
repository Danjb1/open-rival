#pragma once

#include <memory>

#include "net/packet-handlers/PacketHandler.h"

namespace Rival {

class KickPlayerPacketHandler : public PacketHandler
{
public:
    void onPacketReceived(std::shared_ptr<const Packet> packet, State& state) override;
};

}  // namespace Rival
