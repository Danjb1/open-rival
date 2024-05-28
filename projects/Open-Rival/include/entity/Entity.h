#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "entity/components/EntityComponent.h"
#include "game/MapUtils.h"
#include "utils/LogUtils.h"

// Define type traits that can be used to determine if an Entity subclass has a `staticEntityType` member
template <typename, typename = std::void_t<>>
struct has_entity_type : std::false_type
{
    // Primary template will be used if T does not have a member `staticEntityType`
};
template <typename T>
struct has_entity_type<T, std::void_t<decltype(T::staticEntityType)>> : std::true_type
{
    // Specialization that will be used if T does have a member `staticEntityType`
};

namespace Rival {

class World;

enum class EntityType : std::uint8_t
{
    Unknown,
    Unit,
    Building,
    Projectile,
    Tree,
    Mountain,
    Wall,
    Palisade,
    Door,
    Decoration,
    Chest,
    InfoPoint
};

const auto firstEntityType { static_cast<int>(EntityType::Unknown) };
const auto lastEntityType { static_cast<int>(EntityType::InfoPoint) };

/**
 * Base class for a "thing" that exists within the game world.
 *
 * In general, Entity functionality should be placed on Components.
 *
 * The Entity lifecycle looks something like this...
 *
 *     Construction:
 *
 *     - Construct Entity
 *     - attach() components
 *     - addedToWorld() when Entity is added to the world
 *         - id is assigned here
 *         - All components receive onEntityAddedToWorld() callback
 *     - onReady() called (for subclasses)
 *
 *      Game Loop:
 *
 *     - earlyUpdate()
 *     - update()
 *         - Components marked for delete receive destroy() callback
 *         - All components receive update() callback
 *
 *      Destruction:
 *
 *     - markForDelete() to mark an Entity for deletion
 *     - destroy() when Entity is actually deleted
 *         - All components receive destroy() callback
 *
 * Note that, unless specified, no guarantees are made about the order in
 * which Entities within the game world receive lifecycle callbacks,
 * except that this order will always be deterministic.
 */
class Entity : public std::enable_shared_from_this<Entity>
{
public:
    virtual ~Entity() = default;

    /**
     * Attaches an EntityComponent to this Entity.
     *
     * Results in a callback to `EntityComponent.onAttach`.
     */
    void attach(std::shared_ptr<EntityComponent> component);

    /** Called when this Entity is added to the game world. */
    void addedToWorld(World* newScenario, int newId, MapNode newPos);

    /** Called once at the start of each frame. */
    void earlyUpdate();

    /** Updates this Entity by one frame. */
    void update();

    /** Determines if this Entity has been marked for deletion. */
    const bool isDeleted() const
    {
        return deleted;
    }

    /**
     * Marks this Entity for deletion.
     *
     * Entities marked for deletion will be destroyed at an appropriate time
     * during the game loop, and should be considered non-existent for the
     * purposes of logic and rendering.
     */
    void markForDeletion()
    {
        deleted = true;
    }

    /** Called when this Entity is destroyed. */
    void destroy();

    /** Gets a pointer to the World that holds this Entity. */
    World* getWorld()
    {
        return world;
    }

    /** Gets the unique identifier for this Entity. */
    int getId() const
    {
        return id;
    }

    /** Gets the type of this Entity. */
    EntityType getType() const
    {
        return entityType;
    }

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
    const MapNode getPos() const
    {
        return pos;
    }

    /** Gets the number of tiles this Entity occupies in the x-axis. */
    int getWidth() const
    {
        return width;
    }

    /** Gets the number of tiles this Entity occupies in the y-axis. */
    int getHeight() const
    {
        return height;
    }

    /** Moves this Entity to a new position. */
    void setPos(MapNode newPos);

    /** Casts this Entity to the given subclass, if it is safe to do so.
     * Subclasses MUST have a `staticEntityType` member variable in order for this to work.
     * Returns nullptr if there is a type mismatch. */
    template <class T>
    T* as()
    {
        static_assert(std::is_base_of<Entity, T>::value, "T must be a subclass of Entity");
        static_assert(has_entity_type<T>::value, "T must have a static member variable named 'staticEntityType'");
        return entityType == T::staticEntityType ? static_cast<T*>(this) : nullptr;
    }

    /** Casts this Entity to the given subclass, if it is safe to do so (const version).
     * Subclasses MUST have a `staticEntityType` member variable in order for this to work.
     * Returns nullptr if there is a type mismatch. */
    template <class T>
    const T* as() const
    {
        static_assert(std::is_base_of<Entity, T>::value, "T must be a subclass of Entity");
        static_assert(has_entity_type<T>::value, "T must have a static member variable named 'staticEntityType'");
        return entityType == T::staticEntityType ? static_cast<const T*>(this) : nullptr;
    }

    /**
     * Gets a raw pointer to the EntityComponent with the given key (mutable version).
     *
     * This is not safe for long-term storage; the weak/shared pointer versions should be used instead.
     *
     * Returns nullptr if no matching EntityComponent is found.
     */
    template <class T>
    T* getComponent(const std::string& key)
    {
        const auto iter = components.find(key);
        if (iter == components.cend())
        {
            return nullptr;
        }
        return static_cast<T*>((iter->second).get());
    }

    /**
     * Gets a raw pointer to the EntityComponent with the given key (read-only version).
     *
     * This is not safe for long-term storage; the weak/shared pointer versions should be used instead.
     *
     * Returns nullptr if no matching EntityComponent is found.
     */
    template <class T>
    const T* getComponent(const std::string& key) const
    {
        const auto iter = components.find(key);
        if (iter == components.cend())
        {
            return nullptr;
        }
        return static_cast<T*>((iter->second).get());
    }

    /**
     * Gets a shared pointer to the EntityComponent with the given key (mutable version).
     *
     * The weak pointer version should generally be preferred to avoid memory leaks, unless a component explicitly
     * needs to be kept alive.
     *
     * Returns an empty shared_ptr if no matching EntityComponent is found.
     */
    template <class T>
    std::shared_ptr<T> getComponentShared(const std::string& key)
    {
        const auto iter = components.find(key);
        if (iter == components.cend())
        {
            return std::shared_ptr<T>();
        }
        return std::dynamic_pointer_cast<T>(iter->second);
    }

    /**
     * Gets a shared pointer to the EntityComponent with the given key (read-only version).
     *
     * The weak pointer version should generally be preferred to avoid memory leaks, unless a component explicitly
     * needs to be kept alive.
     *
     * Returns an empty shared_ptr if no matching EntityComponent is found.
     */
    template <class T>
    std::shared_ptr<const T> getComponentShared(const std::string& key) const
    {
        const auto iter = components.find(key);
        if (iter == components.cend())
        {
            return std::shared_ptr<T>();
        }
        return std::dynamic_pointer_cast<const T>(iter->second);
    }

    /**
     * Gets a weak pointer to the EntityComponent with the given key (mutable version).
     *
     * Returns an empty weak_ptr if no matching EntityComponent is found.
     */
    template <class T>
    std::weak_ptr<T> getComponentWeak(const std::string key)
    {
        const auto iter = components.find(key);
        if (iter == components.cend())
        {
            return std::weak_ptr<T>();
        }
        return std::dynamic_pointer_cast<T>(iter->second);
    }

    /**
     * Gets a weak pointer to the EntityComponent with the given key (read-only version).
     *
     * Returns an empty weak_ptr if no matching EntityComponent is found.
     */
    template <class T>
    std::weak_ptr<const T> getComponentWeak(const std::string& key) const
    {
        const auto iter = components.find(key);
        if (iter == components.cend())
        {
            return std::weak_ptr<T>();
        }
        return std::dynamic_pointer_cast<const T>(iter->second);
    }

    /** Returns the result of `getComponent`, and verifies that it is valid. */
    template <class T>
    T* requireComponent(const std::string& key)
    {
        auto result = getComponent<T>(key);
        if (!result)
        {
            LOG_WARN("No component found with key: {}", key);
        }
        return result;
    }

    /** Returns the result of `getComponent`, and verifies that it is valid. */
    template <class T>
    const T* requireComponent(const std::string& key) const
    {
        auto result = getComponent<T>(key);
        if (!result)
        {
            LOG_WARN("No component found with key: {}", key);
        }
        return result;
    }

    /** Returns the result of `getComponentShared`, and verifies that it is valid. */
    template <class T>
    std::shared_ptr<T> requireComponentShared(const std::string& key)
    {
        auto result = getComponentShared<T>(key);
        if (!result)
        {
            LOG_WARN("No component found with key: {}", key);
        }
        return result;
    }

    /** Returns the result of `getComponentShared`, and verifies that it is valid. */
    template <class T>
    std::shared_ptr<const T> requireComponentShared(const std::string& key) const
    {
        auto result = getComponentShared<const T>(key);
        if (!result)
        {
            LOG_WARN("No component found with key: {}", key);
        }
        return result;
    }

    /** Returns the result of `getComponentWeak`, and verifies that it is valid. */
    template <class T>
    std::weak_ptr<T> requireComponentWeak(const std::string& key)
    {
        auto result = getComponentWeak<T>(key);
        if (!result.lock())
        {
            LOG_WARN("No component found with key: {}", key);
        }
        return result;
    }

    /** Returns the result of `getComponentWeak`, and verifies that it is valid. */
    template <class T>
    std::weak_ptr<const T> requireComponentWeak(const std::string& key) const
    {
        auto result = getComponentWeak<const T>(key);
        if (!result.lock())
        {
            LOG_WARN("No component found with key: {}", key);
        }
        return result;
    }

    bool operator==(const Entity& other) const
    {
        return id == other.id;
    }

    bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

protected:
    /** Creates an Entity with the given size. */
    Entity(EntityType entityType, int width, int height);

    /** Called when this Entity is added to the game world, after component initialization. */
    virtual void onReady() {}

    /** Called when this Entity is removed from the game world, before component destruction. */
    virtual void onDestroy() {};

public:
    // TODO: This is only relevant to Units and Buildings, maybe move it somewhere else
    static const int maxNameLength = 12;

    /** Flag set if this Entity has moved in the current frame. */
    bool moved;

protected:
    /** Unique identifier used to refer to this Entity. */
    int id;

    /** Type of this Entity. */
    EntityType entityType;

    /** Flag set when this Entity is marked for deletion. */
    bool deleted;

    /** The World this Entity has been added to. */
    World* world;

    /** Co-ordinate of the tile the Entity is occupying. */
    MapNode pos;

    /** Number of tiles this Entity occupies in the x-axis. */
    int width;

    /** Number of tiles this Entity occupies in the y-axis. */
    int height;

    /** EntityComponents owned by this Entity.
     * We use shared_ptrs here so that we can create weak_ptrs to
     * components, although in practice the Entity is the sole owner. */
    std::unordered_map<std::string, std::shared_ptr<EntityComponent>> components;
};

}  // namespace Rival
