#pragma once

#include <string>

#include "EntityComponent.h"

namespace Rival {

/**
 * Component that links an entity to a player.
 */
class OwnerComponent : public EntityComponent
{

public:
    static const std::string key;

    OwnerComponent(int player);

    int getPlayer() const;

private:
    int player;
};

}  // namespace Rival
