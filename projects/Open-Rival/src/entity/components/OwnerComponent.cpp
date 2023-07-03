#include "entity/components/OwnerComponent.h"

namespace Rival {

const std::string OwnerComponent::key = "owner";

OwnerComponent::OwnerComponent(int playerId)
    : EntityComponent(key)
    , playerId(playerId)
{
}

}  // namespace Rival
