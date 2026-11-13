#pragma once

namespace Rival {

/**
 * Tracks an iteration over a collection for as long as it is in scope.
 *
 * This increments the given counter on construction and decrements it on destruction (even if an exception is
 * thrown). The counter can then be checked to determine whether an iteration is in progress, e.g. to avoid modifying
 * the collection. Using a counter rather than a flag allows iterations to be nested.
 */
class IterationGuard
{
public:
    explicit IterationGuard(int& depth)
        : depth(depth)
    {
        ++depth;
    }

    ~IterationGuard()
    {
        --depth;
    }

    IterationGuard(const IterationGuard&) = delete;
    IterationGuard& operator=(const IterationGuard&) = delete;

private:
    int& depth;
};

}  // namespace Rival
