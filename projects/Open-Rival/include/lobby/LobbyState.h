#pragma once

#include <memory>
#include <unordered_map>

#include "net/packets/Packet.h"
#include "State.h"

namespace Rival {

class Application;
class PacketHandler;

/**
 * Application state active when in-game.
 */
class LobbyState : public State
{

public:
    LobbyState(Application& app);

    // Begin State override
    void onLoad() override;
    void update() override;
    void render(int delta) override;
    // End State override

    void startGame();

private:
    std::unique_ptr<State> createGameState() const;

private:
    /** Registered PacketHandlers by packet type. */
    std::unordered_map<PacketType, std::unique_ptr<PacketHandler>> packetHandlers;
};

}  // namespace Rival
