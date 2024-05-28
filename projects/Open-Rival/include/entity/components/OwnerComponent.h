#pragma once

#include <string>

#include "entity/components/EntityComponent.h"

namespace Rival {

/**
 * Component that links an entity to a player.
 */
class OwnerComponent : public EntityComponent
{

public:
    OwnerComponent(int playerId);

    int getPlayerId() const
    {
        return playerId;
    }

public:
    static const std::string key;

private:
    int playerId;
};

}  // namespace Rival
