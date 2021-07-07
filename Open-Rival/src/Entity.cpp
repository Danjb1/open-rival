#include "pch.h"
#include "Entity.h"

namespace Rival {

    Entity::Entity(int width, int height)
        : width(width),
          height(height),
          x(-1),
          y(-1),
          scenario(nullptr),
          deleted(false),
          moved(true),
          id(-1) {}

    void Entity::attach(std::unique_ptr<EntityComponent> component) {
        component->onAttach(this);
        components.insert({ component->getKey(), std::move(component) });
    }

    void Entity::onSpawn(
            Scenario* newScenario, int newId, int newX, int newY) {
        scenario = newScenario;
        id = newId;
        x = newX;
        y = newY;

        for (auto const& kv : components) {
            const auto& component = kv.second;
            component->onEntitySpawned(scenario);
        }
    }

    void Entity::earlyUpdate() {
        moved = false;
    }

    void Entity::update() {
        // Update our Components
        for (auto const& kv : components) {
            const auto& component = kv.second;
            if (!component->isDeleted()) {
                component->update();
            }
        }

        // Remove deleted Components
        for (auto it = components.begin(); it != components.end();) {
            const auto& component = it->second;
            if (component->isDeleted()) {
                it = components.erase(it);
            } else {
                ++it;
            }
        }
    }

}  // namespace Rival
