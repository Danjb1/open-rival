#pragma once

#include <vector>

#include "net/packets/Packet.h"

namespace Rival {

/**
 * A packet that can be forwarded without needing to be parsed.
 */
class AnonymousPacket : public Packet
{
public:
    AnonymousPacket(const std::vector<char>& buffer);

    void serialize(std::vector<char>& buffer) const override;

private:
    const std::vector<char> packetData;
};

}  // namespace Rival
