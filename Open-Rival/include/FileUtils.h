#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <cstdint>
#include <string>
#include <vector>

#include "json.h"

using json = nlohmann::json;

namespace Rival { namespace FileUtils {

    std::vector<std::uint8_t> readBinaryFile(std::string filename);

    std::string readTextFile(std::string filename);

    json readJsonFile(std::string filename);

}}  // namespace Rival::FileUtils

#endif  // FILE_UTILS_H
