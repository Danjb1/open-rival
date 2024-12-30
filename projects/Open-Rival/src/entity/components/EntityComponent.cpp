#include "entity/components/EntityComponent.h"

namespace Rival {

EntityComponent::EntityComponent(std::string key)
    : _key(key)
{
}

void EntityComponent::onAttach(Entity* e)
{
    entity = e;
}

std::weak_ptr<EntityComponent> EntityComponent::getWeakThis()
{
    return std::dynamic_pointer_cast<EntityComponent>(shared_from_this());
}

}  // namespace Rival
