#include "entity/components/ArmorComponent.h"

namespace Rival {

const std::string ArmorComponent::key = "armor";

ArmorComponent::ArmorComponent(int armor)
    : EntityComponent(key)
    , armor(armor)
{
}

int ArmorComponent::getArmor() const
{
    return armor;
}

}  // namespace Rival
