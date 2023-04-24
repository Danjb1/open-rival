#pragma once

#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

/**
 * A packet that wraps a received packet, for forwarding by the relay server.
 *
 * The original packet data is not important for the server, and is never inspected.
 */
class AnonymousPacket : public Packet
{
public:
    AnonymousPacket(const std::vector<char>& buffer, int playerId);

    void serialize(std::vector<char>& buffer) const override;

private:
    const std::vector<char> packetData;
};

}  // namespace Rival
