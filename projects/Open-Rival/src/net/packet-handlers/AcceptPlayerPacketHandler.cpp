#include "pch.h"

#include "net/packet-handlers/AcceptPlayerPacketHandler.h"

#include "lobby/LobbyState.h"
#include "net/ClientInfo.h"
#include "net/packets/AcceptPlayerPacket.h"

namespace Rival {

void AcceptPlayerPacketHandler::onPacketReceived(std::shared_ptr<const Packet> packet, State& state)
{
    std::shared_ptr<const AcceptPlayerPacket> acceptPacket = std::static_pointer_cast<const AcceptPlayerPacket>(packet);

    LobbyState& lobby = static_cast<LobbyState&>(state);

    ClientInfo client(acceptPacket->getPlayerId(), acceptPacket->getPlayerName());
    lobby.onPlayerAccepted(acceptPacket->getRequestId(), acceptPacket->getClientId(), client);
}

}  // namespace Rival
