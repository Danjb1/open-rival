#include "catch2/catch.h"

#include <memory>

#include "entity/Unit.h"
#include "entity/components/EntityComponent.h"

using namespace Rival;

namespace TestEntity {

class ExampleEntityComponent : public EntityComponent
{
public:
    bool attached;
    bool entitySpawned;
    int& updateCount;

    ExampleEntityComponent(int& updateCount)
        : EntityComponent("example_key")
        , attached(false)
        , entitySpawned(false)
        , updateCount(updateCount)
    {
    }

    void onAttach(Entity* e) override
    {
        EntityComponent::onAttach(e);
        attached = true;
    }

    void onEntityAddedToWorld(World*) override
    {
        entitySpawned = true;
    }

    void update() override
    {
        ++updateCount;
    }
};

SCENARIO("Entities can have components attached to them", "[entity]")
{
    GIVEN("An Entity")
    {
        Unit e(UnitType::Knight, "", false);

        WHEN("attaching a component to the Entity")
        {
            int updateCount = 0;
            e.attach(std::make_shared<ExampleEntityComponent>(updateCount));
            const ExampleEntityComponent* component = e.requireComponent<ExampleEntityComponent>("example_key");

            THEN("the component is attached to the Entity")
            {
                REQUIRE(component != nullptr);
            }

            THEN("the component receives a callback")
            {
                REQUIRE(component->attached);
            }
        }
    }
}

SCENARIO("Entities should initialize their components when they spawn", "[entity]")
{
    GIVEN("An Entity with a component")
    {
        Unit e(UnitType::Knight, "", false);
        int updateCount = 0;
        e.attach(std::make_shared<ExampleEntityComponent>(updateCount));

        WHEN("the Entity is spawned")
        {
            e.addedToWorld(nullptr, 0, { 0, 0 });

            THEN("the component receives a callback")
            {
                const ExampleEntityComponent* component = e.requireComponent<ExampleEntityComponent>("example_key");
                REQUIRE(component->entitySpawned);
            }
        }
    }
}

SCENARIO("Entities should update their components each frame", "[entity]")
{
    GIVEN("An Entity with a component")
    {
        Unit e(UnitType::Knight, "", false);
        int updateCount = 0;
        e.attach(std::make_shared<ExampleEntityComponent>(updateCount));

        WHEN("the Entity is updated")
        {
            e.update();

            THEN("the component receives a callback")
            {
                const ExampleEntityComponent* component = e.requireComponent<ExampleEntityComponent>("example_key");
                REQUIRE(component->updateCount == 1);
            }
        }

        AND_GIVEN("A component is deleted")
        {
            ExampleEntityComponent* component = e.requireComponent<ExampleEntityComponent>("example_key");
            component->markForDeletion();

            WHEN("the Entity is updated")
            {
                e.update();

                THEN("the deleted component is not updated")
                {
                    REQUIRE(updateCount == 0);
                }

                THEN("the deleted component is removed from the Entity")
                {
                    const ExampleEntityComponent* foundComponent =
                            e.getComponent<ExampleEntityComponent>("example_key");
                    REQUIRE(foundComponent == nullptr);
                }
            }
        }
    }
}

SCENARIO("Entities can return components by their keys", "[entity]")
{
    GIVEN("An Entity with a component")
    {
        Unit e(UnitType::Knight, "", false);
        int updateCount = 0;
        e.attach(std::make_shared<ExampleEntityComponent>(updateCount));

        WHEN("retrieving the component by its key")
        {
            const ExampleEntityComponent* component = e.requireComponent<ExampleEntityComponent>("example_key");

            THEN("the component is returned")
            {
                REQUIRE(component != nullptr);
            }
        }

        WHEN("trying to retrieve a non-existent component")
        {
            const ExampleEntityComponent* component = e.getComponent<ExampleEntityComponent>("no_such_key");

            THEN("the a nullptr is returned")
            {
                REQUIRE(component == nullptr);
            }
        }
    }
}

}  // namespace TestEntity
