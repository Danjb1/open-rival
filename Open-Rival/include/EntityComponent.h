#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include <string>

namespace Rival {

    // Forward declaration to avoid circular reference
    class Entity;

    /**
     * Class used to encapsulate some Entity behaviour.
     */
    class EntityComponent {

    public:
        EntityComponent(std::string key);
        virtual ~EntityComponent() {}

        /**
         * Callback for when this Component is attached to an Entity.
         */
        virtual void onAttach(Entity* e);

        /**
         * Callback for when the owning Entity is spawned.
         *
         * At this point, the Entity is guaranteed to have a valid ID and
         * position.
         */
        virtual void onEntitySpawned() {}

        /**
         * Updates this EntityComponent by one frame.
         */
        virtual void update() {};

        /**
         * Determines if this EntityComponent has been marked for deletion.
         */
        const bool isDeleted() const;

        /**
         * Marks this EntityComponent for deletion.
         *
         * EntityComponents marked for deletion will be deleted during Entity
         * processing, and should be considered non-existent for the purposes
         * of logic and rendering.
         */
        void markForDeletion();

        /**
         * Gets the key used to store and retrieve this EntityComponent.
         */
        std::string getKey();

    protected:
        /**
         * The Entity that owns this EntityComponent.
         */
        Entity* entity;

    private:
        /**
         * Key used to store and retrieve this EntityComponent.
         */
        std::string _key;

        /**
         * Flag set when this EntityComponent is marked for deletion.
         */
        bool deleted;
    };

}  // namespace Rival

#endif  // ENTITY_COMPONENT_H
