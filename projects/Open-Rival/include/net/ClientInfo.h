#pragma once

#include <string>

namespace Rival {

class ClientInfo
{
public:
    ClientInfo() {}
    ClientInfo(int playerId, std::string name);

    int getPlayerId() const
    {
        return playerId;
    }

    std::string getName() const
    {
        return name;
    }

public:
    static constexpr int maxNameLength = 16;

private:
    int playerId = -1;
    std::string name;
};

}  // namespace Rival
