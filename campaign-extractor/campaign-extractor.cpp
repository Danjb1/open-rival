#include "pch.h"

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "FileUtils.h"

namespace Rival { namespace Setup {

    uint32_t readInt(std::vector<std::uint8_t>& data, size_t offset)
    {
        // little endian
        return std::uint32_t(
                data[offset + 3] << 24 | data[offset + 2] << 16 | data[offset + 1] << 8 | data[offset + 0]);
    }

    void extractCampaign(std::string filename)
    {
        auto data = FileUtils::readBinaryFile(filename);

        // Read the number of levels
        size_t pos = 0;
        std::uint8_t numLevels = std::uint8_t(data[pos]);
        pos++;

        // Extract each level in turn
        for (int i = 0; i < numLevels; i++)
        {

            // Read the scenario offset and size
            std::uint32_t offset = readInt(data, pos);
            pos += 4;
            std::uint32_t size = readInt(data, pos);
            pos += 4;

            // Determine the output filename
            std::ostringstream outputFilename;
            outputFilename << filename.substr(0, filename.length() - 4) << "-" << std::setw(2) << std::setfill('0')
                           << (i + 1) << ".sco";

            // Prepare to write to a new file
            std::ofstream output(outputFilename.str(), std::ios::binary);
            if (!output.is_open())
            {
                throw std::runtime_error("Failed to open file for writing");
            }

            // Write the scenario data
            output.write(reinterpret_cast<char*>(&data[offset]), size);
        }
    }

}}  // namespace Rival::Setup
