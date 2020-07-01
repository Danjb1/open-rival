#include "pch.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

uint32_t readInt(std::vector<unsigned char>& data, size_t offset) {
    // little endian
    return std::uint32_t(
            data[offset + 3] << 24
            | data[offset + 2] << 16
            | data[offset + 1] << 8
            | data[offset + 0]);
}

void extractCampaign(std::string filename) {

    // Open the file at the end
    std::ifstream is(filename, std::ios::binary | std::ios::ate);
    if (!is.is_open()) {
        throw std::runtime_error("Failed to open scenario: " + filename);
    }

    // Create a buffer to hold the entire file contents
    std::streampos size = is.tellg();
    if (size == -1) {
        throw std::runtime_error("Failed to retrieve file size");
    }
    std::vector<unsigned char> data(static_cast<size_t>(size));

    // Read the entire file to memory
    is.seekg(0, std::ios::beg);
    is.read(reinterpret_cast<char*>(data.data()), size);
    is.close();

    // Read the number of levels
    size_t pos = 0;
    std::uint8_t numLevels = std::uint8_t(data[pos]);
    pos++;

    // Extract each level in turn
    for (int i = 0; i < numLevels; i++) {

        // Read the scenario offset and size
        std::uint32_t offset = readInt(data, pos);
        pos += 4;
        std::uint32_t size = readInt(data, pos);
        pos += 4;

        // Determine the output filename
        std::ostringstream outputFilename;
        outputFilename
                << filename.substr(0, filename.length() - 4)
                << "-"
                << std::setw(2)
                << std::setfill('0')
                << (i + 1)
                << ".sco";

        // Prepare to write to a new file
        std::ofstream output(outputFilename.str(), std::ios::binary);
        if (!output.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the scenario data
        output.write(reinterpret_cast<char*>(&data[offset]), size);
        output.close();
    }
}

int main() {
    extractCampaign("CELF.CMP");
    extractCampaign("CGRE.CMP");
    extractCampaign("CHUMAN.CMP");
    extractCampaign("INTRO.CMP");
    extractCampaign("SORR.BLD");
}
