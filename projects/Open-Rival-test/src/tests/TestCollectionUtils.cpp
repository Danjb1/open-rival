#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <vector>

#include "utils/CollectionUtils.h"

using namespace Rival;

namespace TestCollectionUtils {

struct Item
{
    int id;
};

/** Returns the IDs of all items visited by a single iteration. */
std::vector<int> visit(WeakPtrSet<Item>& set)
{
    std::vector<int> visitedIds;
    set.forEach([&](std::shared_ptr<Item> item) { visitedIds.push_back(item->id); });
    return visitedIds;
}

SCENARIO("WeakPtrSet iterates items in the order they were added", "[collection-utils]")
{
    GIVEN("A WeakPtrSet containing several items")
    {
        // Allocate the items in reverse, to make it unlikely that address order matches insertion order
        auto c = std::make_shared<Item>(3);
        auto b = std::make_shared<Item>(2);
        auto a = std::make_shared<Item>(1);

        WeakPtrSet<Item> set;
        set.emplace(b);
        set.emplace(a);
        set.emplace(c);

        THEN("the items are visited in insertion order")
        {
            REQUIRE(visit(set) == std::vector<int> { 2, 1, 3 });
        }

        WHEN("an item is added again")
        {
            set.emplace(a);

            THEN("it is not duplicated")
            {
                REQUIRE(visit(set) == std::vector<int> { 2, 1, 3 });
            }
        }

        WHEN("an item is removed")
        {
            set.erase(a);

            THEN("it is no longer visited")
            {
                REQUIRE(visit(set) == std::vector<int> { 2, 3 });
                REQUIRE(!set.contains(a));
            }
        }

        WHEN("an item expires")
        {
            b.reset();

            THEN("it is no longer visited")
            {
                REQUIRE(visit(set) == std::vector<int> { 1, 3 });
            }
        }
    }
}

SCENARIO("WeakPtrSet can be modified during iteration", "[collection-utils]")
{
    GIVEN("A WeakPtrSet containing several items")
    {
        auto a = std::make_shared<Item>(1);
        auto b = std::make_shared<Item>(2);
        auto c = std::make_shared<Item>(3);

        WeakPtrSet<Item> set;
        set.emplace(a);
        set.emplace(b);
        set.emplace(c);

        WHEN("items are removed during iteration")
        {
            std::vector<int> visitedIds;
            set.forEach([&](std::shared_ptr<Item> item) {
                visitedIds.push_back(item->id);
                if (item->id == 1)
                {
                    // Remove a later item, and the current item
                    set.erase(c);
                    set.erase(a);
                }
            });

            THEN("removed items are not visited")
            {
                REQUIRE(visitedIds == std::vector<int> { 1, 2 });
            }

            THEN("removed items are gone after the iteration")
            {
                REQUIRE(visit(set) == std::vector<int> { 2 });
            }
        }

        WHEN("an item is added during iteration")
        {
            auto d = std::make_shared<Item>(4);

            std::vector<int> visitedIds;
            set.forEach([&](std::shared_ptr<Item> item) {
                visitedIds.push_back(item->id);
                set.emplace(d);
            });

            THEN("the new item is not visited during that iteration")
            {
                REQUIRE(visitedIds == std::vector<int> { 1, 2, 3 });
            }

            THEN("the new item is visited during the next iteration")
            {
                REQUIRE(visit(set) == std::vector<int> { 1, 2, 3, 4 });
            }
        }
    }
}

}  // namespace TestCollectionUtils
