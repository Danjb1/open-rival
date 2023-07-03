#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Rival { namespace FileUtils {

std::vector<std::uint8_t> readBinaryFile(std::string filename);

std::string readTextFile(std::string filename);

}}  // namespace Rival::FileUtils
