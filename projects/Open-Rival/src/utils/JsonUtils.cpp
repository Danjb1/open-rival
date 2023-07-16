#include "utils/JsonUtils.h"

#include <fstream>

#include "utils/LogUtils.h"

namespace Rival { namespace JsonUtils {

json readJsonFile(std::string filename)
{
    LOG_TRACE("Reading JSON file: {}", filename);
    std::ifstream is(filename);
    json j;
    is >> j;
    return j;
}

}}  // namespace Rival::JsonUtils
