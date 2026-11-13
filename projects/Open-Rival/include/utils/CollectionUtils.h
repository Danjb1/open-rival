#pragma once

#include <algorithm>
#include <cstddef>  // std::size_t
#include <functional>
#include <memory>
#include <vector>

#include "utils/IterationGuard.h"

namespace Rival {

/**
 * A set of weak_ptrs, typically used to store listeners.
 *
 * Items are iterated in the order they were added. Having a deterministic order is important so that all clients
 * simulate the game in the exact same way.
 *
 * It is safe to add or remove items while iterating:
 *  - Items removed during iteration are not visited.
 *  - Items added during iteration are not visited until the next iteration.
 *
 * Internally, items are stored using a vector, so adding items (including a uniqueness check) and finding items is
 * O(n). In practice, this is fine, since listener sets are typically small.
 */
template <class T>
class WeakPtrSet
{
public:
    /** Adds an item to the set, if it is not already present. */
    void emplace(std::weak_ptr<T> item)
    {
        if (item.expired() || contains(item))
        {
            return;
        }
        items.push_back(std::move(item));
    }

    /** Removes an item from the set, if present. */
    void erase(const std::weak_ptr<T>& item)
    {
        const auto it = findItem(item);
        if (it == items.end())
        {
            return;
        }

        if (iterationDepth > 0)
        {
            // Erasing now would disrupt the ongoing iteration; instead, clear this entry so it gets removed afterwards
            it->reset();
        }
        else
        {
            items.erase(it);
        }
    }

    /** Determines whether the given item is present in the set. */
    bool contains(const std::weak_ptr<T>& item) const
    {
        return findItem(item) != items.cend();
    }

    /** Calls a function for each valid item in the set. Expired items will be removed. */
    void forEach(const std::function<void(std::shared_ptr<T>)>& func)
    {
        {
            IterationGuard guard(iterationDepth);

            // Items added during iteration will be appended to the end; skip these until the next iteration
            const std::size_t numItems = items.size();
            for (std::size_t i = 0; i < numItems; ++i)
            {
                // Copy the shared_ptr before calling `func`, since `items` may be modified during the call
                if (std::shared_ptr<T> item = items[i].lock())
                {
                    func(item);
                }
            }
        }

        if (iterationDepth == 0)
        {
            std::erase_if(items, [](const std::weak_ptr<T>& item) { return item.expired(); });
        }
    }

private:
    typename std::vector<std::weak_ptr<T>>::iterator findItem(const std::weak_ptr<T>& item)
    {
        return std::find_if(items.begin(), items.end(), [&](const auto& other) { return isSameOwner(item, other); });
    }

    typename std::vector<std::weak_ptr<T>>::const_iterator findItem(const std::weak_ptr<T>& item) const
    {
        return std::find_if(items.cbegin(), items.cend(), [&](const auto& other) { return isSameOwner(item, other); });
    }

    static bool isSameOwner(const std::weak_ptr<T>& a, const std::weak_ptr<T>& b)
    {
        return !a.owner_before(b) && !b.owner_before(a);
    }

private:
    std::vector<std::weak_ptr<T>> items;
    int iterationDepth = 0;
};

namespace CollectionUtils {

/**
 * Iterates over a set of weak_ptrs and calls a function for each valid item in the list.
 * Expired pointers will be removed.
 *
 * USAGE:
 * CollectionUtils::forEachWeakPtr<MyType>(listeners, [&](auto& elem) { elem->doSomething(); });
 */
template <typename T>
void forEachWeakPtr(WeakPtrSet<T>& collection, const std::function<void(std::shared_ptr<T>)>& func)
{
    collection.forEach(func);
}

}  // namespace CollectionUtils

}  // namespace Rival
