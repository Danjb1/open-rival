#pragma once

#include <memory>
#include <string>
#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

/**
 * Packet sent by a new player after connecting to the server.
 */
class RequestJoinPacket : public Packet
{
public:
    RequestJoinPacket(int requestId, std::string playerName);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<RequestJoinPacket> deserialize(const std::vector<char> buffer);

    std::string getPlayerName() const
    {
        return playerName;
    }

    int getRequestId() const
    {
        return requestId;
    }

private:
    int requestId;
    std::string playerName;
};

}  // namespace Rival
