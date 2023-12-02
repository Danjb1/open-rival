#pragma once

#include <memory>
#include <set>

// Second parameter tells the set to use a custom comparison function,
// which uses the comparison functions of the pointed-to values
template <class T>
using WeakPtrSet = std::set<std::weak_ptr<T>, std::owner_less<std::weak_ptr<T>>>;
