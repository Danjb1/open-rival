#include "catch2/catch.h"

#include "entity/Entity.h"
#include "entity/components/PassabilityComponent.h"
#include "entity/components/WalkerComponent.h"
#include "game/Pathfinding.h"
#include "game/World.h"
#include "utils/TimeUtils.h"

using namespace Rival;

namespace TestWalkerComponent {

SCENARIO("WalkerComponent can plan a route", "[components][movement-component]")
{     

    GIVEN("A unit with a WalkerComponent")
    {
        World world(5, 5, false);
        Entity e(EntityType::Unit, 1, 1);
        e.attach(std::make_shared<WalkerComponent>());
        e.onSpawn(&world, 0, { 1, 1 });

        WalkerComponent* walkerComponent = e.requireComponent<WalkerComponent>(WalkerComponent::key);

        WHEN("trying to plan a route to a reachable tile")
        {
            Pathfinding::Context context;
            walkerComponent->moveTo({ 1, 2 }, context);

            THEN("a route is found to the destination")
            {
                REQUIRE(!walkerComponent->getRoute().isEmpty());
                MapNode expectedDestination = { 1, 2 };
                REQUIRE(walkerComponent->getRoute().getFinalDestination() == expectedDestination);
            }
        }
    }
    
    GIVEN("A unit with a WalkerComponent in an enclosed space")
    {
        World world(6, 6, false);
        Entity e(EntityType::Unit, 1, 1);
        e.attach(std::make_shared<WalkerComponent>());
        e.onSpawn(&world, 0, { 3, 2 });

        WalkerComponent* walkerComponent = e.requireComponent<WalkerComponent>(WalkerComponent::key);

        const MapNode enclosure[] = {
            { 1, 2 }, { 2, 2 }, { 3, 1 }, { 4, 1 }, { 2, 3 }, { 5, 1 }, { 3, 3 }, { 4, 3 }, { 5, 2 }, { 6, 2 } 
        };
        for (const MapNode& node : enclosure)
        {
            world.setPassability(node, TilePassability::Blocked);
        }

        WHEN("Trying to pathfind out of the enclosure")
        {
            Pathfinding::Context context;
            walkerComponent->moveTo({ 5, 5 }, context);

            THEN("It plans a route to the closest reachable tile to the destination")
            {
                REQUIRE(!walkerComponent->getRoute().isEmpty());
                MapNode expectedDestination = { 4, 2 };
                REQUIRE(walkerComponent->getRoute().getFinalDestination() == expectedDestination);
            }
        }
    }
    
    GIVEN("A unit with a WalkerComponent positioned in the corner of a large map")
    {
        World largeWorld(1001, 1001, false);
        Entity e(EntityType::Unit, 1, 1);
        e.attach(std::make_shared<WalkerComponent>());
        e.onSpawn(&largeWorld, 0, { 1, 1 });

        WalkerComponent* walkerComponent = e.requireComponent<WalkerComponent>(WalkerComponent::key);

        WHEN("Trying to pathfind to the opposite corner") 
        {
            Pathfinding::Context context;
            walkerComponent->moveTo({ 1000, 1000 }, context);

            THEN("A partial route is returned, respecting the limits of the pathfinder")
            {
                REQUIRE(!walkerComponent->getRoute().isEmpty());
                REQUIRE(walkerComponent->getRoute().getFinalDestination()
                        != walkerComponent->getRoute().getIntendedDestination());
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
        Pathfinding::Context context;
        walkerComponent->moveTo(destination, context);

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
        Pathfinding::Context context;
        walkerComponent->moveTo(destination, context);

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
