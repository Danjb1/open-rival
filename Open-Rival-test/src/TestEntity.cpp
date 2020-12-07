#include "pch.h"
#include "catch2/catch.h"

#include <memory>

#include "Entity.h"
#include "EntityComponent.h"

using namespace Rival;

class ExampleEntityComponent : public EntityComponent {
public:
    bool attached;
    bool entitySpawned;
    int& updateCount;

    ExampleEntityComponent(int& updateCount)
        : EntityComponent("example_key"),
          updateCount(updateCount) {}

    void onAttach(Entity* e) override {
        EntityComponent::onAttach(e);
        attached = true;
    }

    void onEntitySpawned(Scenario* scenario) override {
        entitySpawned = true;
    }

    void update() override {
        ++updateCount;
    }
};

SCENARIO("Entities can have components attached to them", "[entity]") {

    GIVEN("An Entity") {
        Entity e(1, 1);

        WHEN("attaching a component to the Entity") {
            int updateCount = 0;
            e.attach(std::make_unique<ExampleEntityComponent>(updateCount));
            ExampleEntityComponent* component =
                    e.getComponent<ExampleEntityComponent>("example_key");

            THEN("the component is attached to the Entity") {
                REQUIRE(component != nullptr);
            }

            AND_THEN("the component receives a callback") {
                REQUIRE(component->attached);
            }
        }
    }
}

SCENARIO("Entities should initialise their components when they spawn", "[entity]") {

    GIVEN("An Entity with a component") {
        Entity e(1, 1);
        int updateCount = 0;
        e.attach(std::make_unique<ExampleEntityComponent>(updateCount));

        WHEN("the Entity is spawned") {
            e.onSpawn(nullptr, 0, 0, 0);

            THEN("the component receives a callback") {
                ExampleEntityComponent* component =
                        e.getComponent<ExampleEntityComponent>("example_key");
                REQUIRE(component->entitySpawned);
            }
        }
    }
}

SCENARIO("Entities should update their components each frame", "[entity]") {

    GIVEN("An Entity with a component") {
        Entity e(1, 1);
        int updateCount = 0;
        e.attach(std::make_unique<ExampleEntityComponent>(updateCount));

        WHEN("the Entity is updated") {
            e.update();

            THEN("the component receives a callback") {
                ExampleEntityComponent* component =
                        e.getComponent<ExampleEntityComponent>("example_key");
                REQUIRE(component->updateCount == 1);
            }
        }

        AND_GIVEN("A component is deleted") {
            ExampleEntityComponent* component =
                    e.getComponent<ExampleEntityComponent>("example_key");
            component->markForDeletion();

            WHEN("the Entity is updated") {
                e.update();

                THEN("the deleted component is not updated") {
                    REQUIRE(updateCount == 0);
                }

                AND_THEN("the deleted component is removed from the Entity") {
                    component = e.getComponent<ExampleEntityComponent>(
                            "example_key");
                    REQUIRE(component == nullptr);
                }
            }
        }
    }
}

SCENARIO("Entities can return components by their keys", "[entity]") {

    GIVEN("An Entity with a component") {
        Entity e(1, 1);
        int updateCount = 0;
        e.attach(std::make_unique<ExampleEntityComponent>(updateCount));

        WHEN("retrieving the component by its key") {
            ExampleEntityComponent* component =
                    e.getComponent<ExampleEntityComponent>("example_key");

            THEN("the component is returned") {
                REQUIRE(component != nullptr);
            }
        }

        AND_WHEN("trying to retrieve a non-existent component") {
            ExampleEntityComponent* component =
                    e.getComponent<ExampleEntityComponent>("no_such_key");

            THEN("the a nullptr is returned") {
                REQUIRE(component == nullptr);
            }
        }
    }
}