#pragma once

#include <memory>
#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

class GameCommand;
class GameCommandFactory;

/**
 * A packet containing all the commands a player has issued for a given tick.
 */
class GameCommandPacket : public Packet
{
public:
    GameCommandPacket(std::vector<std::shared_ptr<GameCommand>> commands, int tick);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<GameCommandPacket>
    deserialize(const std::vector<char> buffer, const GameCommandFactory& commandFactory);

    std::vector<std::shared_ptr<GameCommand>> getCommands() const
    {
        return commands;
    }

    int getTick() const
    {
        return tick;
    }

private:
    std::vector<std::shared_ptr<GameCommand>> commands;
    int tick;
};

}  // namespace Rival
