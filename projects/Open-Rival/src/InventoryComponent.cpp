#include "InventoryComponent.h"

namespace Rival {

const std::string InventoryComponent::key = "inventory";

InventoryComponent::InventoryComponent()
    : EntityComponent(key)
{
}

}  // namespace Rival
