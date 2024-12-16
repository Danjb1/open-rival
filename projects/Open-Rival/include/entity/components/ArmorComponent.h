#pragma once

#include <string>

#include "entity/components/EntityComponent.h"
#include "utils/CollectionUtils.h"

namespace Rival {

/**
 * Component that keeps track of Armor.
 */
class ArmorComponent : public EntityComponent
{
public:
    ArmorComponent(int armor);

    int getArmor() const;

public:
    static const std::string key;

private:
    int armor = 0;
};

}  // namespace Rival
