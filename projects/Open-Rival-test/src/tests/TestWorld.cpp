#include <catch2/catch_test_macros.hpp>

#include <memory>

#include "entity/Unit.h"
#include "game/World.h"
#include "utils/EntityTestUtils.h"

using namespace Rival;

namespace TestWorld {

SCENARIO("Entities added while iterating over the World are deferred", "[world]")
{
    GIVEN("A World containing an Entity")
    {
        World world(5, 5, false);
        world.addEntity(EntityTestUtils::makeUnit(), 1, 1);

        WHEN("an Entity is added while iterating over Entities")
        {
            std::shared_ptr<Unit> newEntity = EntityTestUtils::makeUnit();
            int numEntitiesVisited = 0;

            world.forEachMutableEntity([&](const auto&) {
                ++numEntitiesVisited;
                world.addEntity(newEntity, 2, 2);
            });

            THEN("the Entity is not added immediately")
            {
                REQUIRE(numEntitiesVisited == 1);
                REQUIRE(world.getEntities().size() == 1);
            }

            AND_WHEN("pending Entities are added")
            {
                world.addPendingEntities();

                THEN("the Entity is present in the World")
                {
                    REQUIRE(world.getEntities().size() == 2);
                    REQUIRE(world.getEntitiesAt({ 2, 2 }).size() == 1);
                }
            }
        }

        WHEN("an Entity is requested to be added while iterating over Entities")
        {
            world.forEachMutableEntity([&](const auto&) { world.requestAddEntity(EntityTestUtils::makeUnit(), 2, 2); });

            THEN("the Entity is added along with the other pending Entities")
            {
                REQUIRE(world.getEntities().size() == 1);
                world.addPendingEntities();
                REQUIRE(world.getEntities().size() == 2);
            }
        }
    }
}

SCENARIO("Entities cannot be removed while iterating over the World", "[world]")
{
    GIVEN("A World containing an Entity")
    {
        World world(5, 5, false);
        std::shared_ptr<Unit> entity = EntityTestUtils::makeUnit();
        world.addEntity(entity, 1, 1);

        WHEN("the Entity is removed while iterating over Entities")
        {
            world.forEachMutableEntity([&](const auto& e) { world.removeEntity(e); });

            THEN("the Entity is still present")
            {
                REQUIRE(world.getEntities().size() == 1);
            }
        }

        WHEN("the Entity is removed after iterating over Entities")
        {
            world.forEachMutableEntity([](const auto&) {});
            world.removeEntity(entity);

            THEN("the Entity is removed")
            {
                REQUIRE(world.getEntities().empty());
            }
        }
    }
}

SCENARIO("Entities cannot be added outside the World", "[world]")
{
    GIVEN("An empty World")
    {
        World world(5, 5, false);

        WHEN("Entities are added outside the map")
        {
            world.addEntity(EntityTestUtils::makeUnit(), -1, 0);
            world.addEntity(EntityTestUtils::makeUnit(), 0, -1);
            world.addEntity(EntityTestUtils::makeUnit(), 5, 0);
            world.addEntity(EntityTestUtils::makeUnit(), 0, 5);

            THEN("they are not added")
            {
                REQUIRE(world.getEntities().empty());
            }
        }

        WHEN("an Entity is added at the edge of the map")
        {
            world.addEntity(EntityTestUtils::makeUnit(), 4, 4);

            THEN("it is added")
            {
                REQUIRE(world.getEntities().size() == 1);
            }
        }
    }
}

}  // namespace TestWorld
