#include "pch.h"
#include "WaveFile.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace Rival {

    // Based on:
    // https://github.com/zxf8665905/wavDecoder
    WaveFile::WaveFile(std::string filename) {
        std::cout << "Loading: " << filename << "\n";

        // Open file and check size
        std::streamoff remainSize;
        std::ifstream in(filename,
                std::ios::in | std::ios::binary | std::ios::ate);
        if (in.is_open() == false) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        remainSize = in.tellg();

        // Read WAV header
        if (remainSize <= sizeof(header)) {
            throw std::runtime_error("WAV file is too small: " + filename);
        }
        in.seekg(0, std::ios::beg);
        in.read(reinterpret_cast<char*>(&header), sizeof(header));

        // Determine the remaining size
        remainSize -= sizeof(header);
        if (remainSize < header.Subchunk2Size) {
            throw std::runtime_error("WAV file is too small: " + filename);
        }
        size_t soundDataSize = header.Subchunk2Size;

        // Read the rest of the file
        std::vector<std::uint8_t> soundData;
        soundData.reserve(soundDataSize);
        in.read(reinterpret_cast<char*>(soundData.data()), soundDataSize);
        in.close();
    }

}  // namespace Rival
