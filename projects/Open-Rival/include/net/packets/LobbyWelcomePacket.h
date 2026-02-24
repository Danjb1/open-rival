#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "net/ClientInfo.h"
#include "net/packets/Packet.h"

namespace Rival {

/**
 * Packet sent by the host when a new player joins, to inform them of the current lobby state.
 */
class LobbyWelcomePacket : public Packet
{
public:
    LobbyWelcomePacket(int playerId, std::unordered_map<int, ClientInfo> clients, unsigned int randomSeed);

    void serialize(std::vector<char>& buffer) const override;
    static std::shared_ptr<LobbyWelcomePacket> deserialize(const std::vector<char> buffer);

    int getPlayerId() const
    {
        return playerId;
    }

    std::unordered_map<int, ClientInfo> getClients() const
    {
        return clients;
    }

    unsigned int getRandomSeed() const
    {
        return randomSeed;
    }

private:
    int playerId;
    std::unordered_map<int, ClientInfo> clients;
    unsigned int randomSeed;
};

}  // namespace Rival
