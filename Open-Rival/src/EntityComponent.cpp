#include "pch.h"
#include "EntityComponent.h"

namespace Rival {

    EntityComponent::EntityComponent(std::string key)
        : _key(key) {}

    void EntityComponent::onAttach(Entity* e) {
        entity = e;
    }

}  // namespace Rival
