#include "pch.h"

#include "StringUtils.h"

#include <string>

// Disable warnings about deprecated functionality, until we implement a better
// solution; currently there is no standard way to convert between wstring and
// UTF-8.
#pragma warning(push)
#pragma warning(disable : 4996)

namespace Rival { namespace StringUtils {

// Based on: https://stackoverflow.com/a/12903901/1624459
std::string toUtf8(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

// Based on: https://stackoverflow.com/a/12903901/1624459
std::wstring toWstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(str);
}

}}  // namespace Rival::StringUtils

#pragma warning(pop)
