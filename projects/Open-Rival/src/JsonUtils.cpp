#include "JsonUtils.h"

#include <fstream>

namespace Rival { namespace JsonUtils {

json readJsonFile(std::string filename)
{
    std::ifstream is(filename);
    json j;
    is >> j;
    return j;
}

}}  // namespace Rival::JsonUtils
