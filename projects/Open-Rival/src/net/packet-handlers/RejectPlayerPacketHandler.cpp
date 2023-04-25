#include "pch.h"

#include "net/packet-handlers/RejectPlayerPacketHandler.h"

#include "lobby/LobbyState.h"
#include "net/packets/RejectPlayerPacket.h"

namespace Rival {

void RejectPlayerPacketHandler::onPacketReceived(std::shared_ptr<const Packet> packet, State& state)
{
    std::shared_ptr<const RejectPlayerPacket> rejectPacket = std::static_pointer_cast<const RejectPlayerPacket>(packet);

    LobbyState& lobby = static_cast<LobbyState&>(state);
    lobby.onPlayerRejected(rejectPacket->getRequestId(), rejectPacket->getPlayerName());
}

}  // namespace Rival
