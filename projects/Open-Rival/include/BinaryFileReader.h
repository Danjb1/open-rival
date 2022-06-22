#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace Rival {

class BinaryFileReader
{

public:
    BinaryFileReader(const std::string filename);

    void skip(int n);

    std::streampos getPos();

    void setPos(std::streampos pos);

    std::uint8_t readByte();

    std::uint16_t readShort();

    /**
     * Reads bytes into the given buffer until it is full or there are no
     * bytes left to read.
     *
     * This will overwrite the contents of the buffer.
     */
    void read(std::vector<std::uint8_t>* buffer);

private:
    std::ifstream in;
};

}  // namespace Rival
