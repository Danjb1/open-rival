#pragma once

#include <memory>
#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

class GameCommand;

/**
 * A packet containing all the commands a player has issued for a given tick.
 */
class GameCommandPacket : public Packet
{
public:
    GameCommandPacket(std::vector<std::shared_ptr<GameCommand>> commands, int tick);

    static std::shared_ptr<GameCommandPacket> deserialize(const std::vector<char> buffer);
    void serialize(std::vector<char>& buffer) const override;

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
