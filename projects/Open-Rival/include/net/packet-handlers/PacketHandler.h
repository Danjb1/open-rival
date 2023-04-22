#pragma once

#include <memory>

#include "net/packets/Packet.h"
#include "State.h"

namespace Rival {

class PacketHandler
{
public:
    virtual ~PacketHandler() {}

    virtual void onPacketReceived(std::shared_ptr<const Packet> packet, State& state) = 0;
};

}  // namespace Rival
