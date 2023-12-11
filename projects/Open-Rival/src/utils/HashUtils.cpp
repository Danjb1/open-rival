#include "utils/HashUtils.h"

namespace Rival { namespace HashUtils {

std::size_t hashCombine(std::size_t lhs, std::size_t rhs)
{
    // Based on boost::hash_combine.
    // See here for a detailed explanation: https://stackoverflow.com/a/27952689/1624459
    if constexpr (sizeof(std::size_t) >= 8)
    {
        lhs ^= rhs + 0x517cc1b727220a95 + (lhs << 6) + (lhs >> 2);
    }
    else
    {
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    }
    return lhs;
}

}}  // namespace Rival::HashUtils
