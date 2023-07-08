#include "net/packet-handlers/GameCommandPacketHandler.h"

#include <vector>

#include "commands/GameCommand.h"
#include "game/GameState.h"
#include "net/packets/GameCommandPacket.h"

namespace Rival {

void GameCommandPacketHandler::onPacketReceived(std::shared_ptr<const Packet> packet, State& state)
{
    std::shared_ptr<const GameCommandPacket> commandPacket = std::static_pointer_cast<const GameCommandPacket>(packet);
    std::vector<std::shared_ptr<GameCommand>> commands = commandPacket->getCommands();
    int tick = commandPacket->getTick();

    GameState& game = static_cast<GameState&>(state);
    game.onClientReady(tick, commandPacket->getClientId());

    for (auto& command : commands)
    {
        game.scheduleCommand(command, tick);
    }
}

}  // namespace Rival
