#include "entity/Entity.h"

namespace Rival {

Entity::Entity(EntityType entityType, int width, int height)
    : entityType(entityType)
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
    components.insert({ component->getKey(), component });
}

void Entity::addedToWorld(World* newWorld, int newId, MapNode newPos)
{
    const bool isFirstTime = (id == -1);

    world = newWorld;
    id = newId;
    pos = newPos;

    if (isFirstTime)
    {
        for (const auto& kv : components)
        {
            const auto& component = kv.second;
            component->onEntityFirstAddedToWorld(world);
        }
    }
    else
    {
        for (const auto& kv : components)
        {
            const auto& component = kv.second;
            component->onEntityAddedToWorld(world);
        }
    }

    onReady();
}

void Entity::removedFromWorld()
{
    for (const auto& kv : components)
    {
        const auto& component = kv.second;
        component->onEntityRemovedFromWorld(world);
    }
}

void Entity::earlyUpdate()
{
    moved = false;
}

void Entity::update(int delta)
{
    for (auto it = components.cbegin(); it != components.cend();)
    {
        const auto& component = it->second;
        if (component->isDeleted())
        {
            // Clean up deleted components
            component->destroy();
            it = components.erase(it);
            continue;
        }

        component->update(delta);
        ++it;
    }
}

void Entity::destroy()
{
    onDestroy();

    // Destroy all components
    for (const auto& kv : components)
    {
        const auto& component = kv.second;
        component->destroy();
    }

    components.clear();
}

void Entity::setPos(MapNode newPos)
{
    pos = newPos;
    moved = true;
}

}  // namespace Rival
