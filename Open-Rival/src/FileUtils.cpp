#include "pch.h"

#include "FileUtils.h"

#include <fstream>
#include <sstream>

namespace Rival { namespace FileUtils {

std::vector<std::uint8_t> readBinaryFile(std::string filename)
{
    // Open the file at the end
    std::ifstream is(filename, std::ios::binary | std::ios::ate);
    if (!is.is_open())
    {
        throw std::runtime_error("Failed to open scenario: " + filename);
    }

    // Create a buffer to hold the entire file contents
    std::streampos size = is.tellg();
    if (size == -1)
    {
        throw std::runtime_error("Failed to retrieve file size");
    }
    auto data = std::vector<unsigned char>(static_cast<size_t>(size));

    // Read the entire file to memory
    is.seekg(0, std::ios::beg);
    is.read(reinterpret_cast<char*>(data.data()), size);
    is.close();

    return data;
}

std::string readTextFile(std::string filename)
{
    std::ifstream is(filename);
    std::stringstream buffer;
    buffer << is.rdbuf();
    return buffer.str();
}

json readJsonFile(std::string filename)
{
    std::ifstream is(filename);
    json j;
    is >> j;
    return j;
}

}}  // namespace Rival::FileUtils
