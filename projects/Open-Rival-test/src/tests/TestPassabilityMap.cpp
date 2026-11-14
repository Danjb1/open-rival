#include <catch2/catch_test_macros.hpp>

#include <array>
#include <random>
#include <vector>

#include "game/MapUtils.h"
#include "game/PassabilityMap.h"
#include "game/Tile.h"

using namespace Rival;

namespace TestPassabilityMap {

constexpr int mapWidth = 6;
constexpr int mapHeight = 5;

// A wall across the full width of the map at this row splits the map into 2 regions.
// (This works because no movement can cross more than 1 row at a time.)
constexpr int wallRow = 2;
const MapNode north(1, 0);
const MapNode south(4, 4);

/** Adds the given flags to every tile in the wall row, except the given gap (if any). */
void buildWall(PassabilityMap& map, TilePassability flags, int gapX = -1)
{
    for (int x = 0; x < mapWidth; ++x)
    {
        if (x != gapX)
        {
            map.addPassability({ x, wallRow }, flags);
        }
    }
}

SCENARIO("PassabilityMap determines reachability between tiles", "[passability-map]")
{
    GIVEN("A map where reachability has not been initialized")
    {
        PassabilityMap map(mapWidth, mapHeight, TilePassability::Clear);

        THEN("querying reachability is an error")
        {
            REQUIRE_THROWS(map.isReachable(north, south));
        }
    }

    GIVEN("A map with no obstacles")
    {
        PassabilityMap map(mapWidth, mapHeight, TilePassability::Clear);
        map.initReachability();

        THEN("all tiles are reachable from one another")
        {
            REQUIRE(map.isReachable(north, south));
            REQUIRE(map.isReachable(south, north));
        }

        THEN("tiles outside the map are not reachable")
        {
            REQUIRE(!map.isReachable(north, { -1, 0 }));
            REQUIRE(!map.isReachable(north, { mapWidth, 0 }));
            REQUIRE(!map.isReachable({ 0, mapHeight }, { 0, mapHeight }));
        }
    }

    GIVEN("A map split in 2 by a wall")
    {
        PassabilityMap map(mapWidth, mapHeight, TilePassability::Clear);
        buildWall(map, TilePassability::Blocked);
        map.initReachability();

        THEN("tiles on opposite sides of the wall are not reachable")
        {
            REQUIRE(!map.isReachable(north, south));
        }

        THEN("tiles on the same side of the wall are reachable")
        {
            REQUIRE(map.isReachable(north, { 4, 1 }));
            REQUIRE(map.isReachable(south, { 0, 3 }));
        }

        THEN("the wall tiles are not reachable from clear tiles")
        {
            REQUIRE(!map.isReachable(north, { 0, wallRow }));
        }
    }

    GIVEN("A map with land and water")
    {
        PassabilityMap map(mapWidth, mapHeight, TilePassability::Clear);
        buildWall(map, TilePassability::Water);
        map.initReachability();

        THEN("water tiles are not reachable from land")
        {
            REQUIRE(!map.isReachable(north, { 0, wallRow }));
        }

        THEN("water tiles are reachable from other connected water tiles")
        {
            REQUIRE(map.isReachable({ 0, wallRow }, { mapWidth - 1, wallRow }));
        }
    }
}

SCENARIO("PassabilityMap updates reachability when tiles change", "[passability-map]")
{
    GIVEN("A map with no obstacles")
    {
        PassabilityMap map(mapWidth, mapHeight, TilePassability::Clear);
        map.initReachability();

        WHEN("a building is placed in the middle of the map")
        {
            const MapNode building(2, 2);
            map.addPassability(building, TilePassability::Building);

            THEN("tiles around the building are still reachable from one another")
            {
                REQUIRE(map.isReachable(north, south));
                REQUIRE(map.isReachable({ 2, 1 }, { 2, 3 }));
            }

            THEN("the building is not reachable from clear tiles")
            {
                REQUIRE(!map.isReachable(north, building));
            }

            AND_WHEN("another building is placed next to it")
            {
                const MapNode otherBuilding(2, 3);
                map.addPassability(otherBuilding, TilePassability::Building);

                THEN("the buildings are part of the same group")
                {
                    REQUIRE(map.isReachable(building, otherBuilding));
                }
            }

            AND_WHEN("the building is removed")
            {
                map.removePassability(building, TilePassability::Building);

                THEN("the tile is reachable again")
                {
                    REQUIRE(map.isReachable(north, building));
                }
            }
        }
    }

    GIVEN("A map split by a wall with 2 gaps")
    {
        const MapNode gap1(1, wallRow);
        const MapNode gap2(4, wallRow);

        PassabilityMap map(mapWidth, mapHeight, TilePassability::Clear);
        buildWall(map, TilePassability::Blocked);
        map.removePassability(gap1, TilePassability::Blocked);
        map.removePassability(gap2, TilePassability::Blocked);
        map.initReachability();

        REQUIRE(map.isReachable(north, south));

        WHEN("one gap is closed")
        {
            map.addPassability(gap1, TilePassability::Building);

            THEN("the 2 sides are still reachable via the other gap")
            {
                REQUIRE(map.isReachable(north, south));
            }

            AND_WHEN("the other gap is closed")
            {
                map.addPassability(gap2, TilePassability::Building);

                THEN("the 2 sides are no longer reachable from one another")
                {
                    REQUIRE(!map.isReachable(north, south));
                }

                THEN("tiles on the same side are still reachable from one another")
                {
                    REQUIRE(map.isReachable(north, { 4, 1 }));
                    REQUIRE(map.isReachable(south, { 0, 3 }));
                }

                AND_WHEN("the first gap is reopened")
                {
                    map.removePassability(gap1, TilePassability::Building);

                    THEN("the 2 sides are reachable again")
                    {
                        REQUIRE(map.isReachable(north, south));
                    }
                }
            }
        }
    }

    GIVEN("A map split by a wall with a single gap")
    {
        constexpr int gapX = 3;
        const MapNode gap(gapX, wallRow);

        PassabilityMap map(mapWidth, mapHeight, TilePassability::Clear);
        buildWall(map, TilePassability::Blocked, gapX);
        map.initReachability();

        REQUIRE(map.isReachable(north, south));

        WHEN("a building is placed in the gap")
        {
            map.addPassability(gap, TilePassability::Building);

            THEN("the 2 sides are no longer reachable from one another")
            {
                REQUIRE(!map.isReachable(north, south));
            }

            AND_WHEN("the building is removed")
            {
                map.removePassability(gap, TilePassability::Building);

                THEN("the 2 sides are reachable again")
                {
                    REQUIRE(map.isReachable(north, south));
                }
            }
        }

        WHEN("a unit stands in the gap")
        {
            map.addPassability(gap, TilePassability::GroundUnit);

            THEN("reachability is unaffected")
            {
                REQUIRE(map.isReachable(north, south));
            }
        }

        WHEN("the gap is closed using setPassability")
        {
            map.setPassability(gap, TilePassability::Blocked);

            THEN("the 2 sides are no longer reachable from one another")
            {
                REQUIRE(!map.isReachable(north, south));
            }
        }
    }

    GIVEN("A map where obstacles are added before reachability is initialized")
    {
        // This mirrors how scenarios are loaded: Entities are added first, then reachability is initialized
        PassabilityMap map(mapWidth, mapHeight, TilePassability::Clear);
        map.addPassability({ 0, 0 }, TilePassability::GroundUnit);
        buildWall(map, TilePassability::Building);
        map.initReachability();

        THEN("reachability reflects the obstacles")
        {
            REQUIRE(!map.isReachable(north, south));
        }

        THEN("a tile occupied by a unit is still reachable")
        {
            REQUIRE(map.isReachable({ 0, 0 }, north));
        }
    }
}

/** Counts the pairs of tiles whose reachability differs from a map where reachability is calculated from scratch. */
int countReachabilityDifferences(const PassabilityMap& map)
{
    std::vector<TilePassability> passabilities;
    for (int y = 0; y < map.getHeight(); ++y)
    {
        for (int x = 0; x < map.getWidth(); ++x)
        {
            passabilities.push_back(map.getPassability({ x, y }));
        }
    }

    PassabilityMap freshMap(map.getWidth(), map.getHeight(), passabilities);
    freshMap.initReachability();

    int numDifferences = 0;
    const int numTiles = map.getWidth() * map.getHeight();
    for (int a = 0; a < numTiles; ++a)
    {
        for (int b = a + 1; b < numTiles; ++b)
        {
            const MapNode nodeA(a % map.getWidth(), a / map.getWidth());
            const MapNode nodeB(b % map.getWidth(), b / map.getWidth());
            if (map.isReachable(nodeA, nodeB) != freshMap.isReachable(nodeA, nodeB))
            {
                ++numDifferences;
            }
        }
    }
    return numDifferences;
}

SCENARIO("PassabilityMap keeps reachability correct through many changes", "[passability-map]")
{
    GIVEN("A map where reachability has been initialized")
    {
        constexpr int width = 10;
        constexpr int height = 8;
        PassabilityMap map(width, height, TilePassability::Clear);
        map.initReachability();

        WHEN("many random tiles are changed")
        {
            // Changing these flags can join or split groups (or, in the case of units, have no effect at all)
            const std::array<TilePassability, 3> flagsToToggle = {
                TilePassability::Building, TilePassability::Water, TilePassability::GroundUnit
            };

            // Use a fixed seed so that the test is repeatable
            std::mt19937 rng(1234);
            std::uniform_int_distribution<int> xDist(0, width - 1);
            std::uniform_int_distribution<int> yDist(0, height - 1);
            std::uniform_int_distribution<std::size_t> flagDist(0, flagsToToggle.size() - 1);

            THEN("reachability always matches a map where reachability is calculated from scratch")
            {
                constexpr int numChanges = 300;
                for (int i = 0; i < numChanges; ++i)
                {
                    const MapNode tile(xDist(rng), yDist(rng));
                    const TilePassability flag = flagsToToggle[flagDist(rng)];

                    if ((map.getPassability(tile) & flag) == flag)
                    {
                        map.removePassability(tile, flag);
                    }
                    else
                    {
                        map.addPassability(tile, flag);
                    }

                    INFO("After change " << i << " to tile (" << tile.x << ", " << tile.y << ")");
                    REQUIRE(countReachabilityDifferences(map) == 0);
                }
            }
        }
    }
}

}  // namespace TestPassabilityMap
