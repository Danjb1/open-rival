#include "catch2/catch.h"

#include <algorithm>  // std::any_of

#include "game/SpatialIndex.h"

using namespace Rival;

namespace TestSpatialIndex {

SCENARIO("SpatialIndex can be used to retrieve entities", "[spatialIndex]")
{
    GIVEN("A SpatialIndex containing a handful of Entities")
    {
        SpatialIndex spatialIndex(64, 64);

        std::vector<SpatialEntity> entities = {
            // 2 entities in the same tile
            { 0, { 0, 0 } },
            { 1, { 0, 0 } },

            // 2 entities within radius 1 of {8, 8}, but across chunk boundaries
            { 2, { 7, 7 } },
            { 3, { 9, 8 } },

            // Some other entity, far away
            { 4, { 32, 32 } },
        };

        for (auto& e : entities)
        {
            spatialIndex.addEntity(e);
        }

        WHEN("retrieving Entities in a specific tile")
        {
            std::vector<int> entityIds = spatialIndex.getEntitiesAt({ 0, 0 });

            THEN("only those Entities are returned")
            {
                REQUIRE(entityIds.size() == 2);
                REQUIRE(std::any_of(entityIds.cbegin(), entityIds.cend(), [](auto id) { return id == 0; }));
                REQUIRE(std::any_of(entityIds.cbegin(), entityIds.cend(), [](auto id) { return id == 1; }));
            }
        }

        WHEN("retrieving Entities in a certain radius of a tile")
        {
            std::vector<int> entityIds = spatialIndex.getEntitiesInRadius({ 8, 8 }, 1);

            THEN("only Entities within the radius are returned")
            {
                REQUIRE(entityIds.size() == 2);
                REQUIRE(std::any_of(entityIds.cbegin(), entityIds.cend(), [](auto id) { return id == 2; }));
                REQUIRE(std::any_of(entityIds.cbegin(), entityIds.cend(), [](auto id) { return id == 3; }));
            }
        }
    }
}

SCENARIO("Entities can be removed from a SpatialIndex", "[spatialIndex]")
{
    GIVEN("A SpatialIndex containing an Entity")
    {
        SpatialIndex spatialIndex(64, 64);

        int entityId = 0;
        MapNode pos { 5, 5 };
        spatialIndex.addEntity({ entityId, pos });

        REQUIRE(spatialIndex.getEntitiesAt(pos).size() == 1);

        WHEN("the Entity is removed")
        {
            spatialIndex.removeEntity(entityId);

            THEN("the Entity can no longer be retrieved")
            {
                REQUIRE(spatialIndex.getEntitiesAt(pos).size() == 0);
            }
        }
    }
}

SCENARIO("Entities can be moved within a SpatialIndex", "[spatialIndex]")
{
    GIVEN("A SpatialIndex containing an Entity")
    {
        SpatialIndex spatialIndex(64, 64);

        int entityId = 0;
        MapNode startPos { 5, 5 };
        spatialIndex.addEntity({ entityId, startPos });

        WHEN("the Entity is moved")
        {
            MapNode newPos { 10, 10 };
            spatialIndex.moveEntity(0, newPos);

            THEN("the Entity can no longer be retrieved at the previous location")
            {
                REQUIRE(spatialIndex.getEntitiesAt(startPos).size() == 0);
            }

            THEN("the Entity can be retrieved using the new location")
            {
                REQUIRE(spatialIndex.getEntitiesAt(newPos).size() == 1);
            }
        }
    }
}

}  // namespace TestSpatialIndex
