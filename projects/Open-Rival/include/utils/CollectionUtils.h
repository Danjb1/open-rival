#pragma once

#include <functional>
#include <memory>
#include <set>
#include <vector>

// Second parameter tells the set to use a custom comparison function,
// which uses the comparison functions of the pointed-to values
template <class T>
using WeakPtrSet = std::set<std::weak_ptr<T>, std::owner_less<std::weak_ptr<T>>>;

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
    auto it = collection.begin();
    while (it != collection.end())
    {
        if (std::shared_ptr<T> elem = it->lock())
        {
            func(elem);
            ++it;
        }
        else
        {
            // The weak pointer has expired, erase it from the list
            it = collection.erase(it);
        }
    }
}

}  // namespace CollectionUtils
