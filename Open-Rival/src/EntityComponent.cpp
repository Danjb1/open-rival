#include "pch.h"
#include "EntityComponent.h"

namespace Rival {

    EntityComponent::EntityComponent(std::string key)
        : key(key) {}

    const bool EntityComponent::isDeleted() const {
        return deleted;
    }

    void EntityComponent::markForDeletion() {
        deleted = true;
    }

    void EntityComponent::onAttach(Entity* e) {
        entity = e;
    }

    std::string EntityComponent::getKey() {
        return key;
    }

}  // namespace Rival
