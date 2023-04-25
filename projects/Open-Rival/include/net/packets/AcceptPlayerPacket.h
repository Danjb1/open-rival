#pragma once

#include <memory>
#include <string>
#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

/**
 * Packet sent by the host when accepting a new player.
 */
class AcceptPlayerPacket : public Packet
{
public:
    AcceptPlayerPacket(int requestId, std::string playerName, int playerId);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<AcceptPlayerPacket> deserialize(const std::vector<char> buffer);

    std::string getPlayerName() const
    {
        return playerName;
    }

    int getRequestId() const
    {
        return requestId;
    }

    int getPlayerId() const
    {
        return playerId;
    }

private:
    std::string playerName;
    int requestId;
    int playerId;
};

}  // namespace Rival
