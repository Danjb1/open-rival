#include "pch.h"

#include "WaveFile.h"

#include <cstddef>  // std::size_t
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace Rival {

// Based on:
// https://github.com/zxf8665905/wavDecoder
WaveFile::WaveFile(std::string filename)
    : filename(filename)
{
    // Open file and check size
    std::streamoff remainSize;
    std::ifstream in(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (in.is_open() == false)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    remainSize = in.tellg();

    // Read WAV header
    if (remainSize <= sizeof(header))
    {
        throw std::runtime_error("WAV file is too small: " + filename);
    }
    in.seekg(0, std::ios::beg);
    in.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Determine the remaining size
    remainSize -= sizeof(header);
    if (remainSize < header.subchunk2Size)
    {
        throw std::runtime_error("WAV file is too small: " + filename);
    }
    std::size_t soundDataSize = header.subchunk2Size;

    // Read the rest of the file
    soundData.resize(soundDataSize);
    in.read(reinterpret_cast<char*>(soundData.data()), soundDataSize);

    in.close();
}

}  // namespace Rival
