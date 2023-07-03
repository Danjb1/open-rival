#pragma once

#include "Race.h"

namespace Rival {

/**
 * Game data relating to a player (human or AI).
 */
class PlayerState
{
public:
    PlayerState(Race race, int gold, int wood, int food);

    Race getRace() const
    {
        return race;
    }

private:
    Race race;
    int gold;
    int wood;
    int food;
};

/**
 * Interface providing access to player data.
 */
class PlayerStore
{
public:
    virtual int getNumPlayers() const = 0;
    virtual PlayerState& getLocalPlayerState() const = 0;
    virtual PlayerState* getPlayerState(int playerId) const = 0;
    virtual bool isLocalPlayer(int playerId) const = 0;

public:
    static constexpr int maxPlayers = 8;
};

}  // namespace Rival
