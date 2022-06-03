#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "EntityComponent.h"
#include "MapUtils.h"

namespace Rival {

    class Scenario;

    enum class EntityType : std::uint8_t {
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
     * This is essentially just a "bag of Components":
     * https://gameprogrammingpatterns.com/component.html
     *
     * Note that, unless specified, no guarantees are made about the order in
     * Entities within the game world receive lifecycle callbacks.
     */
    class Entity final {

    public:
        /**
         * Flag set if this Entity has moved in the current frame.
         */
        bool moved;

        /**
         * Creates an Entity with the given size.
         */
        Entity(int width, int height);

        /**
         * Attaches an EntityComponent to this Entity.
         *
         * Results in a callback to `EntityComponent.onAttach`.
         */
        void attach(std::unique_ptr<EntityComponent> component);

        /**
         * Called when this Entity is added to the game world.
         */
        virtual void onSpawn(Scenario* newScenario, int newId, MapNode newPos);

        /**
         * Called once at the start of each frame.
         */
        void earlyUpdate();

        /**
         * Updates this Entity by one frame.
         */
        void update();

        /**
         * Determines if this Entity has been marked for deletion.
         */
        const bool isDeleted() const { return deleted; }

        /**
         * Marks this Entity for deletion.
         *
         * Entities marked for deletion will be deleted at an appropriate time
         * during the game loop, and should be considered non-existent for the
         * purposes of logic and rendering.
         */
        void markForDeletion() { deleted = true; }

        /**
         * Gets a pointer to the Scenario that holds this Entity.
         */
        Scenario* getScenario() { return scenario; }

        /**
         * Gets the unique identifier for this Entity.
         */
        const int getId() const { return id; }

        /**
         * Gets the co-ordinates of the tile this Entity is occupying.
         * 
         * In the case of Entities where width > 0 (e.g. buildings), the x
         * refers to the horizontal centre of the Entity.
         *
         * In the case of Entities where height > 0 (e.g. buildings), the y
         * refers to the bottom corner of the Entity, that is, its
         * bottom-most row.
         *
         * Note that while Entities such as units can move between tiles, their
         * logical position is always represented as a single tile. Movement
         * between tiles is purely visual.
         *
         * In fact, it would not be possible to represent Entity positions as
         * floats because the game world is not a continuous space. Due to the
         * peculiar nature of the isometric tile grid, as x increases, y
         * fluctuates - so it does not make sense to try to store the absolute
         * position of an Entity as a float.
         */
        const MapNode& getPos() const { return pos; }

        /**
         * Gets the number of tiles this Entity occupies in the x-axis.
         */
        int getWidth() const { return width; }

        /**
         * Gets the number of tiles this Entity occupies in the y-axis.
         */
        int getHeight() const { return height; }

        /**
         * Moves this Entity to a new position.
         */
        void setPos(MapNode newPos);

        /**
         * Retrieves the EntityComponent with the given key.
         *
         * Returns nullptr if no matching EntityComponent is found.
         */
        template <class T>
        T* getComponent(std::string key) {
            // Casts away the const from the const version of this method.
            // See: https://stackoverflow.com/a/47369227/1624459
            return const_cast<T*>(std::as_const(*this).getComponent<T>(key));
        }

        /**
         * Retrieves the EntityComponent with the given key.
         *
         * Returns nullptr if no matching EntityComponent is found.
         */
        template <class T>
        const T* getComponent(std::string key) const {
            if (components.find(key) != components.end()) {
                const auto& component = components.at(key);
                return static_cast<T*>(component.get());
            }
            return nullptr;
        }

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
         * The Scenario this Entity has been added to.
         */
        Scenario* scenario;

        /**
         * Co-ordinate of the tile the Entity is occupying.
         */
        MapNode pos;

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
        std::unordered_map<std::string, std::unique_ptr<EntityComponent>> components;
    };

}  // namespace Rival
