#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace Rival { namespace FileUtils {

std::vector<std::uint8_t> readBinaryFile(std::string filename);

std::string readTextFile(std::string filename);

json readJsonFile(std::string filename);

}}  // namespace Rival::FileUtils
