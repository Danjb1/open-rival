#ifndef BINARY_FILE_READER_H
#define BINARY_FILE_READER_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace Rival {

    class BinaryFileReader {

    private:

        std::ifstream in;

    public:

        BinaryFileReader(const std::string filename);

        void skip(int n);

        std::uint8_t readByte();

        std::uint16_t readShort();

        void read(std::vector<unsigned char>* buffer);

    };

}

#endif // BINARY_FILE_READER_H
