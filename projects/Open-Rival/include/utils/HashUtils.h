#pragma once

namespace Rival { namespace HashUtils {

/** Combines 2 hashes to produce a new hash. */
std::size_t hashCombine(std::size_t lhs, std::size_t rhs);

/** Hashes a value, then combines it with the given seed to produce a new hash. */
template <class T>
std::size_t hashCombineObj(std::size_t seed, const T& value)
{
    std::hash<T> hasher;
    std::size_t valueHash = hasher(value);
    return hashCombine(seed, valueHash);
}

}}  // namespace Rival::HashUtils
