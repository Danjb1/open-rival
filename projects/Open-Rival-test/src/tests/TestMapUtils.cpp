#include "catch2/catch.h"

#include "game/MapUtils.h"

using namespace Rival;

namespace TestMapUtils {

TEST_CASE("getDir should return the correct direction from an even-column node", "[map-utils]")
{
    // We are in the top part of the zigzag;
    // => Moving to the row above is diagonally north.
    // => Moving to the *same* row is diagonally south.
    const MapNode startNode = { 10, 10 };

    SECTION("destination is directly North")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x, startNode.y - 1 }) == Facing::North);
    }

    SECTION("destination is directly South")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x, startNode.y + 1 }) == Facing::South);
    }

    SECTION("destination is directly East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 2, startNode.y }) == Facing::East);
    }

    SECTION("destination is directly West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 2, startNode.y }) == Facing::West);
    }

    SECTION("destination is directly North-East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 1, startNode.y - 1 }) == Facing::NorthEast);
    }

    SECTION("destination is directly North-West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 1, startNode.y - 1 }) == Facing::NorthWest);
    }

    SECTION("destination is directly South-East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 1, startNode.y }) == Facing::SouthEast);
    }

    SECTION("destination is directly South-West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 1, startNode.y }) == Facing::SouthWest);
    }
}

TEST_CASE("getDir should return the correct direction from an odd-column node", "[map-utils]")
{
    // We are in the bottom part of the zigzag;
    // => Moving to the *same* row is diagonally north.
    // => Moving to the row below is diagonally south.
    const MapNode startNode = { 11, 10 };

    SECTION("destination is directly North")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x, startNode.y - 1 }) == Facing::North);
    }

    SECTION("destination is directly South")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x, startNode.y + 1 }) == Facing::South);
    }

    SECTION("destination is directly East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 2, startNode.y }) == Facing::East);
    }

    SECTION("destination is directly West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 2, startNode.y }) == Facing::West);
    }

    SECTION("destination is directly North-East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 1, startNode.y }) == Facing::NorthEast);
    }

    SECTION("destination is directly North-West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 1, startNode.y }) == Facing::NorthWest);
    }

    SECTION("destination is directly South-East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 1, startNode.y + 1 }) == Facing::SouthEast);
    }

    SECTION("destination is directly South-West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 1, startNode.y + 1 }) == Facing::SouthWest);
    }
}

TEST_CASE("getDistance should return the correct distance between 2 tiles", "[map-utils]")
{
    // These points were plotted using the Scenario Editor to calculate the expected distance
    const MapNode node1 = { 10, 10 };
    const MapNode node2 = { 11, 10 };
    const MapNode node3 = { 12, 15 };
    const MapNode node4 = { 13, 15 };
    const MapNode node5 = { 21, 15 };
    const MapNode node6 = { 24, 15 };

    SECTION("tiles are east-west neighbours")
    {
        REQUIRE(MapUtils::getDistance(node1, { node1.x + MapUtils::eastWestTileSpan, node1.y }) == 1);
    }

    SECTION("tiles are north-south neighbours")
    {
        REQUIRE(MapUtils::getDistance(node1, { node1.x, node1.y - 1 }) == 1);
    }

    SECTION("tiles are diagonal neighbours (same row)")
    {
        REQUIRE(MapUtils::getDistance(node1, { node1.x + 1, node1.y }) == 1);
    }

    SECTION("tiles are diagonal neighbours (different rows)")
    {
        REQUIRE(MapUtils::getDistance(node1, { node1.x + 1, node1.y - 1 }) == 1);
    }

    SECTION("tiles are in a direct diagonal line (upper -> lower)")
    {
        REQUIRE(MapUtils::getDistance(node1, node5) == 11);
    }

    SECTION("tiles are in a direct diagonal line (lower -> lower)")
    {
        REQUIRE(MapUtils::getDistance(node2, node5) == 10);
    }

    SECTION("tiles are somewhat diagonal, with some extra vertical distance (lower -> upper)")
    {
        REQUIRE(MapUtils::getDistance(node2, node3) == 5);
    }

    SECTION("tiles are somewhat diagonal, with some extra vertical distance (lower -> lower)")
    {
        REQUIRE(MapUtils::getDistance(node2, node4) == 6);
    }

    SECTION("tiles are somewhat diagonal, with some extra vertical distance (upper -> lower)")
    {
        REQUIRE(MapUtils::getDistance(node1, { node1.x + 1, node1.y + 1 }) == 2);
    }

    SECTION("tiles are somewhat diagonal, with some extra horizontal distance (lower -> upper)")
    {
        REQUIRE(MapUtils::getDistance(node2, node6) == 11);
    }

    SECTION("tiles are in a direct horizontal line")
    {
        REQUIRE(MapUtils::getDistance(node4, node5) == 4);
    }

    SECTION("tiles are in the same row but not directly horizontal (upper -> lower)")
    {
        REQUIRE(MapUtils::getDistance(node3, node5) == 5);
    }
}

}  // namespace TestMapUtils
