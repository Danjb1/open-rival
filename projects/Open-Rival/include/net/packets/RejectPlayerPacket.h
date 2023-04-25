#pragma once

#include <memory>
#include <string>
#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

/**
 * Packet sent by the host when rejecting a new player.
 */
class RejectPlayerPacket : public Packet
{
public:
    RejectPlayerPacket(int requestId, std::string playerName);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<RejectPlayerPacket> deserialize(const std::vector<char> buffer);

    std::string getPlayerName() const
    {
        return playerName;
    }

    int getRequestId() const
    {
        return requestId;
    }

private:
    std::string playerName;
    int requestId;
};

}  // namespace Rival
