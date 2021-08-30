#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include <string>
#include <vector>

namespace Rival {
namespace PathUtils {

    /**
     * Gets the default directories to search for fonts.
     */
    std::vector<std::string> getDefaultFontDirs();

}}  // namespace Rival::PathUtils

#endif  // PATH_UTILS_H
