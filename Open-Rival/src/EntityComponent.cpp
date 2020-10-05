#include "pch.h"
#include "EntityComponent.h"

namespace Rival {

    EntityComponent::EntityComponent(std::string key)
        : _key(key) {}

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
        return _key;
    }

}  // namespace Rival
