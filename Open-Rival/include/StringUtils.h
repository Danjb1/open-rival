#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <codecvt>
#include <string>

namespace Rival { namespace StringUtils {

    /**
     * Converts a wide string to a UTF-8 string.
     */
    std::string toUtf8(const std::wstring& wstr);

    /**
     * Converts a UTF-8 string to a wide string.
     *
     * We should use std::string wherever possible, and convert to std::wstring
     * only when needed by library functions.
     *
     * See "UTF-8 Everywhere":
     * http://utf8everywhere.org/
     */
    std::wstring toWstring(const std::string& str);

}}  // namespace Rival::StringUtils

#endif  // STRING_UTILS_H
