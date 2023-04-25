#pragma once

#include <memory>
#include <string>
#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

/**
 * Packet sent by the host when kicking a connected player.
 */
class KickPlayerPacket : public Packet
{
public:
    KickPlayerPacket(int playerId);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<KickPlayerPacket> deserialize(const std::vector<char> buffer);

    int getPlayerId() const
    {
        return playerId;
    }

private:
    int playerId;
};

}  // namespace Rival
