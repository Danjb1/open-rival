#include "utils/JsonUtils.h"

#include <fstream>

#include "utils/LogUtils.h"

namespace Rival { namespace JsonUtils {

json readJsonFile(const std::string& filename)
{
    LOG_TRACE("Reading JSON file: {}", filename);
    json j;
    std::ifstream is(filename);
    if (is.is_open())
    {
        is >> j;
    }
    return j;
}

}}  // namespace Rival::JsonUtils
