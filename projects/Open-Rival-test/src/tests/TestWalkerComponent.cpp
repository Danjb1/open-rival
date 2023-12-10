#include "catch2/catch.h"

#include "entity/Entity.h"
#include "entity/components/PassabilityComponent.h"
#include "entity/components/WalkerComponent.h"
#include "game/World.h"
#include "utils/TimeUtils.h"

using namespace Rival;

namespace TestWalkerComponent {

SCENARIO("WalkerComponent can plan a route", "[components][movement-component]")
{
    World world(5, 5, false);

    GIVEN("A unit with a WalkerComponent")
    {
        Entity e(EntityType::Unit, 1, 1);
        e.attach(std::make_shared<WalkerComponent>());
        e.onSpawn(&world, 0, { 1, 1 });

        WalkerComponent* walkerComponent = e.requireComponent<WalkerComponent>(WalkerComponent::key);

        WHEN("trying to plan a route to a reachable tile")
        {
            walkerComponent->moveTo({ 1, 2 });

            THEN("a route is found to the destination")
            {
                REQUIRE(!walkerComponent->getRoute().isEmpty());
                MapNode expectedDestination = { 1, 2 };
                REQUIRE(walkerComponent->getRoute().getDestination() == expectedDestination);
            }
        }
    }
}

SCENARIO("WalkerComponent can move a unit according to its route", "[components][movement-component]")
{
    World world(5, 5, false);

    GIVEN("A unit with a WalkerComponent and a planned route that is unobstructed")
    {
        Entity e(EntityType::Unit, 1, 1);
        e.attach(std::make_shared<WalkerComponent>());
        e.onSpawn(&world, 0, { 1, 1 });

        WalkerComponent* walkerComponent = e.requireComponent<WalkerComponent>(WalkerComponent::key);
        const MapNode destination = { 1, 2 };
        walkerComponent->moveTo(destination);

        WHEN("the WalkerComponent is updated once")
        {
            walkerComponent->update();

            THEN("the unit starts following the route")
            {
                REQUIRE(walkerComponent->getMovement().isInProgress());
                REQUIRE(walkerComponent->getMovement().timeElapsed == TimeUtils::timeStepMs);
            }

            THEN("the passability of the tiles is updated")
            {
                REQUIRE(world.getPassability({ 1, 1 }) == TilePassability::GroundUnitLeaving);
                REQUIRE(world.getPassability({ 1, 2 }) == TilePassability::GroundUnit);
            }
        }

        WHEN("the WalkerComponent is updated to completion")
        {
            walkerComponent->update();

            while (walkerComponent->getMovement().timeElapsed < walkerComponent->getMovement().timeRequired)
            {
                walkerComponent->update();
            }

            THEN("the unit reaches the destination")
            {
                REQUIRE(!walkerComponent->getMovement().isValid());
                REQUIRE(e.getPos() == destination);
            }

            THEN("the passability of the tiles is updated")
            {
                REQUIRE(world.getPassability({ 1, 1 }) == TilePassability::Clear);
                REQUIRE(world.getPassability({ 1, 2 }) == TilePassability::GroundUnit);
            }
        }
    }

    GIVEN("A unit with a WalkerComponent and a planned route that is now obstructed")
    {
        Entity e(EntityType::Unit, 1, 1);
        e.attach(std::make_shared<WalkerComponent>());
        e.attach(std::make_shared<PassabilityComponent>(TilePassability::GroundUnit));
        e.onSpawn(&world, 0, { 1, 1 });

        // Plan a route
        WalkerComponent* walkerComponent = e.requireComponent<WalkerComponent>(WalkerComponent::key);
        const MapNode destination = { 1, 2 };
        walkerComponent->moveTo(destination);

        // Spawn an obstruction
        Entity obstruction(EntityType::Unit, 1, 1);
        obstruction.attach(std::make_shared<PassabilityComponent>(TilePassability::GroundUnit));
        obstruction.onSpawn(&world, 0, { 1, 2 });

        WHEN("the WalkerComponent is updated")
        {
            walkerComponent->update();

            THEN("the unit does not move")
            {
                REQUIRE(!walkerComponent->getMovement().isValid());
            }

            THEN("the passability of the tiles is unchanged")
            {
                REQUIRE(world.getPassability({ 1, 1 }) == TilePassability::GroundUnit);
                REQUIRE(world.getPassability({ 1, 2 }) == TilePassability::GroundUnit);
            }
        }
    }
}

}  // namespace TestWalkerComponent
