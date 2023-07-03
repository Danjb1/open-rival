#pragma once

#include <string>

#include "entity/components/EntityComponent.h"

namespace Rival {

/**
 * Component that allows an entity to carry items.
 */
class InventoryComponent : public EntityComponent
{

public:
    static const std::string key;

    InventoryComponent();
};

}  // namespace Rival
