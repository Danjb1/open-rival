#include "pch.h"

#include "Entity.h"

namespace Rival {

Entity::Entity(EntityType type, int width, int height)
    : type(type)
    , width(width)
    , height(height)
    , pos({ -1, -1 })
    , world(nullptr)
    , deleted(false)
    , moved(true)
    , id(-1)
{
}

void Entity::attach(std::shared_ptr<EntityComponent> component)
{
    component->onAttach(this);
    components.insert({ component->getKey(), std::move(component) });
}

void Entity::onSpawn(World* newScenario, int newId, MapNode newPos)
{
    world = newScenario;
    id = newId;
    pos = newPos;

    for (auto const& kv : components)
    {
        const auto& component = kv.second;
        component->onEntitySpawned(world);
    }
}

void Entity::earlyUpdate()
{
    moved = false;
}

void Entity::update()
{
    for (auto it = components.cbegin(); it != components.cend();)
    {
        const auto& component = it->second;
        if (component->isDeleted())
        {
            // Clean up deleted components
            component->onDelete();
            it = components.erase(it);
            continue;
        }

        component->update();
        ++it;
    }
}

void Entity::onDelete()
{
    // Delete all components
    for (auto const& kv : components)
    {
        const auto& component = kv.second;
        component->onDelete();
    }

    components.clear();
}

void Entity::setPos(MapNode newPos)
{
    pos = newPos;
    moved = true;
}

}  // namespace Rival
