#include "pch.h"
#include "catch2/catch.h"

#include <memory>

#include "Entity.h"
#include "EntityComponent.h"

using namespace Rival;

class ExampleEntity : public Entity {
public:
    int componentsAttached;
    int componentsUpdated;

    ExampleEntity()
        : Entity(1, 1) {}

    void attach(std::unique_ptr<EntityComponent> component) {
        Entity::attach(std::move(component));
        ++componentsAttached;
    }
};

class ExampleEntityComponent : public EntityComponent {
public:
    bool attached;
    bool entitySpawned;
    int updateCount = 0;

    ExampleEntityComponent()
        : EntityComponent("example_key") {}

    void onAttach(Entity* e) override {
        EntityComponent::onAttach(e);
        attached = true;
    }

    void onEntitySpawned() override {
        entitySpawned = true;
    }

    void update() override {
        ++updateCount;
        ExampleEntity* exampleEntity = dynamic_cast<ExampleEntity*>(entity);
        exampleEntity->componentsUpdated =
                exampleEntity->componentsUpdated + 1;
    }
};

SCENARIO("Entities can have components attached to them", "[entity]") {

    GIVEN("An Entity") {
        ExampleEntity e;

        WHEN("attaching a component to the Entity") {
            e.attach(std::make_unique<ExampleEntityComponent>());

            THEN("the component is attached to the Entity") {
                REQUIRE(e.componentsAttached == 1);
            }

            AND_THEN("the component receives a callback") {
                ExampleEntityComponent* component =
                        dynamic_cast<ExampleEntityComponent*>(
                                e.getComponent("example_key"));
                REQUIRE(component->attached);
            }
        }
    }
}

SCENARIO("Entities should initialise their components when they spawn", "[entity]") {

    GIVEN("An Entity with a component") {
        ExampleEntity e;
        e.attach(std::make_unique<ExampleEntityComponent>());

        WHEN("the Entity is spawned") {
            e.onSpawn(0, 0, 0);

            THEN("the component receives a callback") {
                ExampleEntityComponent* component =
                        dynamic_cast<ExampleEntityComponent*>(
                                e.getComponent("example_key"));
                REQUIRE(component->entitySpawned);
            }
        }
    }
}

SCENARIO("Entities should update their components each frame", "[entity]") {

    GIVEN("An Entity with a component") {
        ExampleEntity e;
        e.attach(std::make_unique<ExampleEntityComponent>());

        WHEN("the Entity is updated") {
            e.update();

            THEN("the component receives a callback") {
                ExampleEntityComponent* component =
                        dynamic_cast<ExampleEntityComponent*>(
                                e.getComponent("example_key"));
                REQUIRE(component->updateCount == 1);
            }
        }

        AND_GIVEN("A component is deleted") {
            ExampleEntityComponent* component =
                    dynamic_cast<ExampleEntityComponent*>(
                            e.getComponent("example_key"));
            component->markForDeletion();

            WHEN("the Entity is updated") {
                e.update();

                THEN("the deleted component is not updated") {
                    REQUIRE(e.componentsUpdated == 0);
                }

                AND_THEN("the deleted component is removed from the Entity") {
                    component = dynamic_cast<ExampleEntityComponent*>(
                            e.getComponent("example_key"));
                    REQUIRE(component == nullptr);
                }
            }
        }
    }
}

SCENARIO("Entities can return components by their keys", "[entity]") {

    GIVEN("An Entity with a component") {
        ExampleEntity e;
        e.attach(std::make_unique<ExampleEntityComponent>());

        WHEN("retrieving the component by its key") {
            ExampleEntityComponent* component =
                    dynamic_cast<ExampleEntityComponent*>(
                            e.getComponent("example_key"));

            THEN("the component is returned") {
                REQUIRE(component != nullptr);
            }
        }

        AND_WHEN("trying to retrieve a non-existent component") {
            ExampleEntityComponent* component =
                    dynamic_cast<ExampleEntityComponent*>(
                            e.getComponent("no_such_key"));

            THEN("the a nullptr is returned") {
                REQUIRE(component == nullptr);
            }
        }
    }
}
