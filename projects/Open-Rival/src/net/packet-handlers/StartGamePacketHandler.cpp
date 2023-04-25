#include "pch.h"

#include "net/packet-handlers/StartGamePacketHandler.h"

#include "lobby/LobbyState.h"
#include "net/packets/StartGamePacket.h"

namespace Rival {

void StartGamePacketHandler::onPacketReceived(std::shared_ptr<const Packet> packet, State& state)
{
    std::shared_ptr<const StartGamePacket> startGamePacket = std::static_pointer_cast<const StartGamePacket>(packet);

    LobbyState& lobby = static_cast<LobbyState&>(state);
    lobby.startGame();
}

}  // namespace Rival
