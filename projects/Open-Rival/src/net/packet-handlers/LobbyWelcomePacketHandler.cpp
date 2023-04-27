#include "pch.h"

#include "net/packet-handlers/LobbyWelcomePacketHandler.h"

#include "lobby/LobbyState.h"
#include "net/packets/LobbyWelcomePacket.h"

namespace Rival {

void LobbyWelcomePacketHandler::onPacketReceived(std::shared_ptr<const Packet> packet, State& state)
{
    std::shared_ptr<const LobbyWelcomePacket> welcomePacket =
            std::static_pointer_cast<const LobbyWelcomePacket>(packet);

    LobbyState& lobby = static_cast<LobbyState&>(state);
    lobby.onWelcomeReceived(welcomePacket->getPlayerId(), welcomePacket->getClients());
}

}  // namespace Rival
