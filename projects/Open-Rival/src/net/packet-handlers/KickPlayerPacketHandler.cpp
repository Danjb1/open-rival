#include "net/packet-handlers/KickPlayerPacketHandler.h"

#include "lobby/LobbyState.h"
#include "net/packets/KickPlayerPacket.h"

namespace Rival {

void KickPlayerPacketHandler::onPacketReceived(std::shared_ptr<const Packet> packet, State& state)
{
    std::shared_ptr<const KickPlayerPacket> kickPacket = std::static_pointer_cast<const KickPlayerPacket>(packet);

    LobbyState& lobby = static_cast<LobbyState&>(state);
    lobby.onPlayerKicked(kickPacket->getPlayerId());
}

}  // namespace Rival
