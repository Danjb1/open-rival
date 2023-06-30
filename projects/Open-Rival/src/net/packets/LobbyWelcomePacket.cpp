#include "net/packets/LobbyWelcomePacket.h"

#include <cstddef>  // std::size_t
#include <string>

#include "utils/BufferUtils.h"
#include "GameCommand.h"

namespace Rival {

LobbyWelcomePacket::LobbyWelcomePacket(int playerId, std::unordered_map<int, ClientInfo> clients)
    : Packet(PacketType::LobbyWelcome)
    , playerId(playerId)
    , clients(clients)
{
}

void LobbyWelcomePacket::serialize(std::vector<char>& buffer) const
{
    Packet::serialize(buffer);

    BufferUtils::addToBuffer(buffer, playerId);

    BufferUtils::addToBuffer(buffer, clients.size());
    for (const auto& entry : clients)
    {
        int thisClientId = entry.first;
        const ClientInfo& client = entry.second;

        BufferUtils::addToBuffer(buffer, thisClientId);
        BufferUtils::addToBuffer(buffer, client.getPlayerId());
        BufferUtils::addStringToBuffer(buffer, client.getName());
    }
}

std::shared_ptr<LobbyWelcomePacket> LobbyWelcomePacket::deserialize(const std::vector<char> buffer)
{
    std::size_t offset = relayedPacketHeaderSize;

    int playerId = 0;
    BufferUtils::readFromBuffer(buffer, offset, playerId);

    std::size_t numClients = 0;
    BufferUtils::readFromBuffer(buffer, offset, numClients);

    std::unordered_map<int, ClientInfo> clients;
    clients.reserve(numClients);

    for (std::size_t i = 0; i < numClients; ++i)
    {
        int thisClientId = 0;
        BufferUtils::readFromBuffer(buffer, offset, thisClientId);

        int clientPlayerId = 0;
        BufferUtils::readFromBuffer(buffer, offset, clientPlayerId);

        std::string name = BufferUtils::readStringFromBuffer(buffer, offset);

        ClientInfo client(playerId, name);
        clients.insert({ thisClientId, client });
    }

    return std::make_shared<LobbyWelcomePacket>(playerId, clients);
}

}  // namespace Rival
