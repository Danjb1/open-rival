#include "pch.h"

#include "net/packet-handlers/RequestJoinPacketHandler.h"

#include "lobby/LobbyState.h"
#include "net/packets/RequestJoinPacket.h"

namespace Rival {

void RequestJoinPacketHandler::onPacketReceived(std::shared_ptr<const Packet> packet, State& state)
{
    std::shared_ptr<const RequestJoinPacket> joinPacket = std::static_pointer_cast<const RequestJoinPacket>(packet);

    LobbyState& lobby = static_cast<LobbyState&>(state);
    lobby.onPlayerJoinRequest(joinPacket->getRequestId(), joinPacket->getClientId(), joinPacket->getPlayerName());
}

}  // namespace Rival
