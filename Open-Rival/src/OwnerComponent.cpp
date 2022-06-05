#include "pch.h"
#include "OwnerComponent.h"

namespace Rival {

    const std::string OwnerComponent::key = "owner";

    OwnerComponent::OwnerComponent(int player) : EntityComponent(key), player(player) {}

    int OwnerComponent::getPlayer() const
    {
        return player;
    }

}  // namespace Rival
