#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>
#include <vector>

#include "EntityComponent.h"

namespace Rival {

    enum class EntityType {
        Unit,
        Building,
        Projectile,
        Tree,
        Mountain,
        Wall,
        Door,
        Decoration,
        Chest,
        InfoPoint
    };

    const auto firstEntityType { static_cast<int>(EntityType::Unit) };
    const auto lastEntityType { static_cast<int>(EntityType::InfoPoint) };

    /**
     * A thing that exists within the game world.
     *
     * This is essentially just a "bag of Components".
     *
     * Further reading:
     * https://gameprogrammingpatterns.com/component.html
     */
    class Entity {

    public:
        Entity(int width, int height);
        virtual ~Entity() {}

        /**
         * Attaches an EntityComponent to this Entity.
         *
         * Results in a callback to `EntityComponent.onAttach`.
         */
        void attach(std::unique_ptr<EntityComponent> component);

        /**
         * Called when this Entity is added to the game world.
         */
        virtual void onSpawn(int newId, int newX, int newY);

        /**
         * Updates this Entity by one frame.
         */
        void update();

        /**
         * Determines if this Entity has been marked for deletion.
         */
        const bool isDeleted() const;

        /**
         * Marks this Entity for deletion.
         *
         * Entities marked for deletion will be deleted at an appropriate time
         * during the game loop, and should be considered non-existent for the
         * purposes of logic and rendering.
         */
        void markForDeletion();

        /**
         * Gets the unique identifier for this Entity.
         */
        const int getId() const;

        /**
         * Gets the x co-ordinate of the tile this Entity is occupying.
         */
        int getX() const;

        /**
         * Gets the y co-ordinate of the tile this Entity is occupying.
         */
        int getY() const;

        /**
         * Retrieves the first EntityComponent with the given key.
         *
         * Returns nullptr if no matching EntityComponent is found.
         */
        EntityComponent* getComponent(std::string key);

    protected:
        /**
         * Unique identifier used to refer to this Entity.
         */
        int id;

        /**
         * Flag set when this Entity is marked for deletion.
         */
        bool deleted;

        /**
         * The type of Entity this represents.
         */
        EntityType type;

        /**
         * x co-ordinate of the tile the Entity is occupying.
         *
         * It does not make sense to store the absolute position of an Entity
         * using floats, because the game world is not a continuous space; due
         * to the peculiar nature of the isometric tile grid, as x increases,
         * y fluctuates.
         *
         * Therefore, we simply store the tile co-ordinates of an Entity, and
         * for Entities moving between tiles, we keep track of the movement
         * direction and a notion of progress towards the destination.
         */
        int x;

        /**
         * y co-ordinate of the tile the Entity is occupying.
         */
        int y;

        /**
         * Number of tiles this Entity occupies in the x-axis.
         */
        int width;

        /**
         * Number of tiles this Entity occupies in the y-axis.
         */
        int height;

        /**
         * EntityComponents owned by this Entity.
         */
        std::vector<std::unique_ptr<EntityComponent>> components;
    };

}  // namespace Rival

#endif  // ENTITY_H
