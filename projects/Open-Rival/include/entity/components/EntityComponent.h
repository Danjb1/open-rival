#pragma once

#include <string>

namespace Rival {

class Entity;
class World;

/**
 * Class used to encapsulate some Entity behaviour.
 *
 * Note that, unless specified, no guarantees are made about the order in
 * which EntityComponents of a given Entity receive lifecycle callbacks,
 * except that this order will always be deterministic.
 */
class EntityComponent
{
    friend class Entity;

public:
    EntityComponent(std::string key);
    virtual ~EntityComponent() = default;

    /**
     * Callback for when this Component is attached to an Entity.
     */
    virtual void onAttach(Entity* e);

    /**
     * Callback for when the owning Entity is spawned.
     *
     * At this point, the Entity is guaranteed to have a valid ID and
     * position, and all initial components will have been attached.
     */
    virtual void onEntityAddedToWorld(World*) {}

    /**
     * Updates this EntityComponent by one frame.
     */
    virtual void update() {};

    /**
     * Determines if this EntityComponent has been marked for deletion.
     */
    const bool isDeleted() const
    {
        return deleted;
    }

    /**
     * Marks this EntityComponent for deletion.
     *
     * EntityComponents marked for deletion will be deleted during Entity
     * processing, and should be considered non-existent for the purposes
     * of logic and rendering.
     */
    void markForDeletion()
    {
        deleted = true;
    }

    /**
     * Gets the key used to store and retrieve this EntityComponent.
     */
    std::string getKey()
    {
        return _key;
    }

protected:
    /**
     * Callback for when this EntityComponent is destroyed.
     */
    virtual void destroy() {};

protected:
    /**
     * The Entity that owns this EntityComponent.
     */
    Entity* entity { nullptr };

private:
    /**
     * Key used to store and retrieve this EntityComponent.
     */
    std::string _key;

    /**
     * Flag set when this EntityComponent is marked for deletion.
     */
    bool deleted { false };
};

}  // namespace Rival
