#include "catch2/catch.h"

#include "entity/Entity.h"
#include "entity/components/WalkerComponent.h"
#include "game/World.h"

using namespace Rival;

namespace TestWalkerComponent {

World world(5, 5, false);

SCENARIO("WalkerComponent can plan a route", "[components][movement-component]")
{
    GIVEN("A unit with a WalkerComponent")
    {
        Entity e(EntityType::Unit, 1, 1);
        e.attach(std::make_shared<WalkerComponent>());
        e.onSpawn(&world, 0, { 1, 1 });

        WalkerComponent* walkerComponent = e.requireComponent<WalkerComponent>(WalkerComponent::key);

        WHEN("trying to plan a route to a reachable tile")
        {
            walkerComponent->moveTo({ 1, 2 });
            walkerComponent->update();

            THEN("a route is found")
            {
                REQUIRE(walkerComponent->getMovement().isValid());
            }
        }

        AND_WHEN("trying to plan a route to an unreachable tile")
        {
            THEN("no route is found")
            {
                REQUIRE(false);
            }
        }
    }
}

SCENARIO("WalkerComponent can move a unit according to its route", "[components][movement-component]")
{
    GIVEN("A unit with a WalkerComponent and a planned route that is unobstructed")
    {
        WHEN("the WalkerComponent is updated")
        {
            THEN("the unit starts following the route")
            {
                REQUIRE(false);
            }

            AND_THEN("the passability of the tiles is updated")
            {
                REQUIRE(false);
            }
        }

        AND_WHEN("the WalkerComponent is updated enough times")
        {
            THEN("the unit reaches the destination")
            {
                REQUIRE(false);
            }

            AND_THEN("the passability of the tiles is updated")
            {
                REQUIRE(false);
            }
        }
    }

    GIVEN("A unit with a WalkerComponent and a planned route that is now obstructed")
    {
        WHEN("the WalkerComponent is updated")
        {
            THEN("the unit does not move")
            {
                REQUIRE(false);
            }

            AND_THEN("the passability of the tiles is unchanged")
            {
                REQUIRE(false);
            }
        }
    }
}

}  // namespace TestWalkerComponent
